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

#include <blaster/raster.h>
#include <blaster/constants.h>
#include <blaster/color.h>
#include <blaster/vector.h>
#include <blaster/matrix.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef OPT_SSE

#include <xmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>

#endif


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
    
    bl_color_set(raster->clear_color,0.9f,0.9f,0.9f,1.0f);
    

    return raster;
}

void bl_raster_delete(bl_raster_t* raster)
{

    bl_texture_delete(raster->color_buffer);
    bl_texture_delete(raster->depth_buffer);
    
    bl_matrix_stack_delete(raster->modelview);
    bl_matrix_stack_delete(raster->projection);

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

void bl_raster_set_clear_color(bl_raster_t* raster,float* color)
{
    raster->clear_color[0]=color[0];
    raster->clear_color[1]=color[1];
    raster->clear_color[2]=color[2];
    raster->clear_color[3]=color[3];
}

void bl_raster_clear(bl_raster_t* raster)
{

#ifdef OPT_SSE
    
    uint32_t pixel=bl_color_get_pixel(raster->clear_color);

    size_t len = raster->screen_width*raster->screen_height;

    __m128i* color = raster->color_buffer->data;
    __m128i* depth = raster->depth_buffer->data;
    
    __m128i C = _mm_set1_epi32(pixel);
    __m128i D = _mm_set1_epi16(0xFFFF);
    
    
    for (size_t n=0;n<len/4;n++) {
        
        _mm_store_si128(color,C);
        
        color++;
    }
    
    for (size_t n=0;n<len/8;n++) {
        
        _mm_store_si128(depth,D);
        
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
    

#else
    uint32_t pixel=bl_color_get_pixel(raster->clear_color);

    uint32_t* color = raster->color_buffer->data;
    uint16_t* depth = raster->depth_buffer->data;
    
    
    for (int j=0;j<raster->screen_height;j++) {
        for (int i=0;i<raster->screen_width;i++) {
            color[i+j*raster->screen_width]=pixel;
            depth[i+j*raster->screen_width]=0xFFFF;
        }
    }
#endif
}

void bl_raster_update(bl_raster_t* raster)
{

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

    float eye[4];
    float clip[4];
    float ndc[4];
    int win[4];

    float* source = vbo->data;
    
    size_t inc=vbo->attributes;
    
    const float wl[4] = {
        raster->screen_width/2.0f,
        raster->screen_height/2.0f,
        65535/2.0f,
        0.0f
        };
        

    for (int n=0;n<vbo->size;n++,source+=inc) {
        
        // eye coordinates
        bl_vector_mult(eye,source,raster->modelview->matrix);
        
        //clip coordinates
        bl_vector_mult(clip,eye,raster->projection->matrix);
        
        //w-divide for NDC coordinates
        float w=1.0f/clip[3];
        
        ndc[0]=clip[0]*w;
        ndc[1]=clip[1]*w;
        ndc[2]=clip[2]*w;
        ndc[3]=clip[3]*w;
        
        //bl_vector_copy(ndc,clip);
        //bl_vector_scale(ndc,w);
        
        //clip out of range z
        if (ndc[2]<-1.0f || ndc[2]>1.0f) {
            continue;
        }
        
        // viewport

        win[0]=(ndc[0]*wl[0])+wl[0];
        win[1]=(ndc[1]*wl[1])+wl[1];
        win[2]=(ndc[2]*wl[2])+wl[2];
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
        
        
        uint16_t* zbuffer=raster->depth_buffer->data+(win[0]+win[1]*raster->screen_width);
    
        if (win[2]<*zbuffer) {
            uint32_t pixel=bl_color_get_pixel(source+4);
    
            bl_texture_set_pixel(raster->color_buffer,
                win[0],win[1],
                pixel
            );
            
            
            *zbuffer=win[2];
        }
        
        
        
    }
}

void bl_raster_draw_lines(bl_raster_t* raster, bl_vbo_t* vbo)
{
}
