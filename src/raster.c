/*
    blaster

    Copyright (C) 2016  Enrique Medina Gremaldos <quiqueiii@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <blaster/optimization.h>
#include <blaster/raster.h>
#include <blaster/constants.h>
#include <blaster/color.h>
#include <blaster/vector.h>
#include <blaster/matrix.h>

#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

static void put_fragment(bl_raster_t* raster,uint16_t x,uint16_t y,uint16_t depth,bl_pixel_t pixel)
{
    if (raster->fragment<raster->num_fragments-1) {
        raster->fragments[raster->fragment].x=x;
        raster->fragments[raster->fragment].y=y;
        raster->fragments[raster->fragment].depth=depth;
        raster->fragments[raster->fragment].pixel=pixel.value;
        raster->fragment++;
    }
}

static void bl_raster_alloc_fragments(bl_raster_t* raster)
{
    for (int n=0;n<BL_NUM_CHUNK_FRAGMENTS;n++) {
        raster->fragments[n].size = BL_NUM_FRAGMENTS;
        raster->fragments[n].n = 0;
        raster->fragments[n].buffer = malloc(sizeof(bl_fragment_t)*BL_NUM_FRAGMENTS);
    }
}

static void bl_raster_free_fragments(bl_raster_t* raster)
{
    for (int n=0;n<BL_NUM_CHUNK_FRAGMENTS;n++) {
        raster->fragments[n].size = 0;
        raster->fragments[n].n = 0;
        
        free(raster->fragments[n].buffer);
        raster->fragments[n].buffer = 0;
    }
}

bl_raster_t* bl_raster_new(int width,int height)
{
    bl_raster_t* raster;

    raster=malloc(sizeof(bl_raster_t));

    raster->screen_width=width;
    raster->screen_height=height;

    raster->color_buffer=bl_texture_new(width,height,BL_TEXTURE_U32);
    raster->depth_buffer=bl_texture_new(width,height,BL_TEXTURE_U16);
    
    raster->modelview=bl_matrix_stack_new(32);
    raster->projection=bl_matrix_stack_new(32);
    
    bl_color_set(&raster->clear_color,0.9f,0.9f,0.9f,1.0f);
    
    bl_raster_alloc_fragments(raster);
    
    raster->fragments=malloc(sizeof(bl_fragment_t)*5000000);
    //raster->fragments=aligned_alloc(64,sizeof(bl_fragment_t)*5000000);

    return raster;
}

void bl_raster_delete(bl_raster_t* raster)
{

    bl_texture_delete(raster->color_buffer);
    bl_texture_delete(raster->depth_buffer);
    
    bl_matrix_stack_delete(raster->modelview);
    bl_matrix_stack_delete(raster->projection);

    bl_raster_free_fragments(raster);
    
    free(raster->fragments);
    free(raster);
}

void bl_raster_resize(bl_raster_t* raster,int width,int height)
{
    bl_texture_delete(raster->color_buffer);
    bl_texture_delete(raster->depth_buffer);
    
    raster->screen_width=width;
    raster->screen_height=height;

    raster->color_buffer=bl_texture_new(width,height,BL_TEXTURE_U32);
    raster->depth_buffer=bl_texture_new(width,height,BL_TEXTURE_U16);
}

void bl_raster_set_clear_color(bl_raster_t* raster,bl_color_t* color)
{
    raster->clear_color.r=color->r;
    raster->clear_color.g=color->g;
    raster->clear_color.b=color->b;
    raster->clear_color.a=color->a;
}

#ifdef BLASTER_RASTER_CLEAR_SSE2
void bl_raster_clear(bl_raster_t* raster)
{
    
    uint32_t pixel=bl_color_get_pixel(&raster->clear_color).value;

    size_t len = raster->screen_width*raster->screen_height;

    __m128i* color = raster->color_buffer->data;
    __m128i* depth = raster->depth_buffer->data;
    
    __m128i C = _mm_set1_epi32(pixel);
    __m128i D = _mm_set1_epi16(0xFFFF);
    
    
    for (size_t n=0;n<len/4;n++) {
        
        _mm_stream_si128(color,C);
        
        color++;
    }
    
    for (size_t n=0;n<len/8;n++) {
        
        _mm_stream_si128(depth,D);
        
        depth++;
    }
    
    uint32_t* rcolor = raster->color_buffer->data;
    uint16_t* rdepth = raster->depth_buffer->data;
    
    size_t remain = len%4;
    rcolor+=len-remain;
    
    for (size_t n=0;n<remain;n++) {
        *rcolor=pixel;
        rcolor++;
    }
    
    remain = len%8;
    rdepth+=len-remain;
    
    for (size_t n=0;n<remain;n++) {
        *rdepth=0xFFFF;
        rdepth++;
    }

    //clear fragment buffer
    raster->fragment=0;
}
#endif

#ifdef BLASTER_RASTER_CLEAR_GENERIC
void bl_raster_clear(bl_raster_t* raster)
{
    bl_pixel_t pixel=bl_color_get_pixel(&raster->clear_color);

    uint32_t* color = raster->color_buffer->data;
    uint16_t* depth = raster->depth_buffer->data;
    
    
    for (int j=0;j<raster->screen_height;j++) {
        for (int i=0;i<raster->screen_width;i++) {
            color[i+j*raster->screen_width]=pixel.value;
            depth[i+j*raster->screen_width]=0xFFFF;
        }
    }

    //clear fragment buffer
    raster->fragment=0;
}
#endif

void bl_raster_update(bl_raster_t* raster)
{
    
    for (size_t n=0;n<raster->fragment;n++) {
        size_t offset=raster->fragments[n].x+raster->fragments[n].y*raster->screen_width;
        
        uint16_t* zbuffer=raster->depth_buffer->data;
        zbuffer+=offset;
        
        uint32_t* cbuffer=raster->color_buffer->data;
        cbuffer+=offset;

        if (raster->fragments[n].depth<*zbuffer) {
            
            *zbuffer=raster->fragments[n].depth;
            *cbuffer=raster->fragments[n].pixel;
        }
    }
    
}

int bl_raster_get_width(bl_raster_t* raster)
{
    return raster->screen_width;
}

int bl_raster_get_height(bl_raster_t* raster)
{
    return raster->screen_height;
}

void bl_raster_draw_points(bl_raster_t* raster,bl_vbo_t* vbo)
{

    // point rendering

    bl_vector_t clip;
    bl_vector_t ndc;
    uint32_t win[4];
    
    typedef struct {
        bl_vector_t pos;
        bl_color_t color;
    } point_t;

    point_t* source = (point_t*) vbo->data;
    
    
    bl_vector_t wl;
    
    wl.x=raster->screen_width/2.0f;
    wl.y=raster->screen_height/2.0f;
    wl.z=65535/2.0f;
    wl.w=0.0f;
    
    
    bl_matrix_t matrix;
    
    // precompute modelview and projection matrix
    bl_matrix_mult(&matrix,raster->projection->matrix,raster->modelview->matrix);
    
    for (int n=0;n<vbo->size;n++) {
        
        //clip coordinates
        bl_vector_mult(&clip,&source[n].pos,&matrix);
        //__builtin_prefetch(color);
        //w-divide for NDC coordinates
        float w=1.0f/clip.w;
        
        ndc.x=clip.x*w;
        ndc.y=clip.y*w;
        ndc.z=clip.z*w;
        ndc.w=clip.w*w;
        
        //bl_vector_copy(ndc,clip);
        //bl_vector_scale(ndc,w);
        
        //clip out of range z
        if (ndc.z<-1.0f || ndc.z>1.0f) {
            continue;
        }
        
        // viewport (FMA here?)
        win[0]=(ndc.x*wl.x)+wl.x;
        win[1]=(ndc.y*wl.y)+wl.y;
        win[2]=(ndc.z*wl.z)+wl.z;
        
        /*
        __m128 N;
        __m128 W;
        __m128i CI;
        
        N=_mm_loadu_ps(ndc);
        W=_mm_loadu_ps(wl);
        N=_mm_mul_ps(N,W);
        N=_mm_add_ps(N,W);
        
        CI = _mm_cvtps_epi32(N);
        _mm_storeu_si128(win,CI);
        */
        //clip out of viewport
        if (win[0]<0 || win[0]>=raster->screen_width
         || win[1]<0 || win[1]>=raster->screen_height) {
            continue;
        }
        
        bl_pixel_t pixel=bl_color_get_pixel(&source[n].color);

        put_fragment(raster,win[0],win[1],win[2],pixel);
        
        /*
        bl_pixel_t pixel=bl_color_get_pixel(&source[n].color);
        win[3]=pixel.value;
      
        __m128i W;
        W=_mm_loadu_si128(win);
        _mm_stream_si128((__m128i*)fragment,W);
        */
        /*
        __m128i W;
        W=_mm_loadu_si128(win);
        _mm_storeu_si128((uint32_t*)fragment,W);
        */
        
        /*
        uint16_t* zbuffer=raster->depth_buffer->data;
        zbuffer+=(win[0]+win[1]*raster->screen_width);

        if (win[2]<*zbuffer) {
            bl_pixel_t pixel=bl_color_get_pixel(&source[n].color);
           
            bl_texture_set_pixel(raster->color_buffer,
                win[0],win[1],
                pixel
            );
            
            
            *zbuffer=win[2];
        }
        */
        
        
    }
    
}

static void swap(uint32_t* a,uint32_t* b)
{
    uint32_t tmp = *a;
    *a=*b;
    *b=tmp;
}

void bl_raster_draw_lines(bl_raster_t* raster, bl_vbo_t* vbo)
{
    typedef struct {
        bl_vector_t pos;
        bl_color_t color;
    } point_t;

    point_t* source = (point_t*) vbo->data;
    

    bl_matrix_t matrix;
    
    // precompute modelview and projection matrix
    bl_matrix_mult(&matrix,raster->projection->matrix,raster->modelview->matrix);
    
    bl_vector_t clip[2];
    bl_vector_t ndc[2];
    
    bl_vector_t wl;
    
    wl.x=raster->screen_width/2.0f;
    wl.y=raster->screen_height/2.0f;
    wl.z=65535/2.0f;
    wl.w=0.0f;
    
    uint32_t w1[4];
    uint32_t w2[4];
    
    for (int n=0;n<vbo->size;n+=2) {
        bl_vector_mult(&clip[0],&source[n].pos,&matrix);
        bl_vector_mult(&clip[1],&source[n+1].pos,&matrix);
        
        float w=1.0f/clip[0].w;
        
        ndc[0].x=clip[0].x*w;
        ndc[0].y=clip[0].y*w;
        ndc[0].z=clip[0].z*w;
        ndc[0].w=clip[0].w*w;
        
        w=1.0f/clip[1].w;
        
        ndc[1].x=clip[1].x*w;
        ndc[1].y=clip[1].y*w;
        ndc[1].z=clip[1].z*w;
        ndc[1].w=clip[1].w*w;
        
        //HACK, lines should be tesselated
        if (ndc[0].z<-1.0f || ndc[0].z>1.0f || ndc[1].z<-1.0f || ndc[1].z>1.0f) {
            continue;
        }
        
        w1[0]=(ndc[0].x*wl.x)+wl.x;
        w1[1]=(ndc[0].y*wl.y)+wl.y;
        w1[2]=(ndc[0].z*wl.z)+wl.z;
        
        w2[0]=(ndc[1].x*wl.x)+wl.x;
        w2[1]=(ndc[1].y*wl.y)+wl.y;
        w2[2]=(ndc[1].z*wl.z)+wl.z;
        
        float rz1 =1.0f/ndc[0].z;
        float rz2 =1.0f/ndc[1].z;
        
        int x1 = w1[0];
        int y1 = w1[1];
        
        int x2 = w2[0];
        int y2 = w2[1];
        
        int xlimit=raster->screen_width-1;
        int ylimit=raster->screen_height-1;
        
        
        const int step = abs(y2-y1) > abs(x2-x1);
        
        if (step) {
            swap(&x1,&y1);
            swap(&x2,&y2);
            swap(&xlimit,&ylimit);
        }
        
        if (x1>x2) {
            swap(&x1,&x2);
            swap(&y1,&y2);
        }
        
        if (x1>xlimit || y1>ylimit) {
            continue;
        }
        
        if (x2<0 || y2<0) {
            continue;
        }
        
        x1=MAX(x1,0);
        x2=MIN(x2,xlimit);
        y1=MAX(y1,0);
        y2=MIN(y2,ylimit);
        
        const int dx = x2 - x1;
        const int dy = abs(y2 - y1);
        
        int error = dx / 2;
        const int ystep = (y1 < y2) ? 1 : -1;
        int y = y1;
        
        const int maxX = x2;
        
        bl_pixel_t pixel=bl_color_get_pixel(&source[n].color);
        
        for (int x=x1; x<maxX; x++) {
            
            if (step) {
                put_fragment(raster,y,x,w1[2],pixel);
            }
            else {
                put_fragment(raster,x,y,w1[2],pixel);
            }
            
            error -= dy;
            if (error < 0) {
                y += ystep;
                error += dx;
            }
            
            
        }
        
    }
}

void bl_raster_draw_triangles(bl_raster_t* raster, bl_vbo_t* vbo)
{
}
