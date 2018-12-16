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

/*
    Internal raster functions
*/

/*!
    Clear buffers (color and depth)
*/
void bl_raster_clear_buffers(bl_raster_t* raster);

/*!
    Draws a vertex buffer as points
*/
void bl_raster_draw_points(bl_raster_t* raster, bl_vbo_t* vbo, size_t start);

/*!
    Draws a vertex bufferas lines
*/
void bl_raster_draw_lines(bl_raster_t* raster, bl_vbo_t* vbo, size_t start);

/*!
    Draws a vertex buffer as triangles
*/
void bl_raster_draw_triangles(bl_raster_t* raster, bl_vbo_t* vbo, size_t start);

void bl_raster_update_chunk(bl_raster_t* raster,bl_fragment_chunk_t* chunk);

static int max_i32(int a,int b)
{
    if (a>b) {
        return a;
    }
    else {
        return b;
    }
}

static int min_i32(int a,int b)
{
    if (a<b) {
        return a;
    }
    else {
        return b;
    }
}

static float rcp(float x)
{
#ifdef BLASTER_RASTER_RCP_SSE
    __m128 V;
    
    V=_mm_set_ss(x);
    V=_mm_rcp_ss(V);
    
    float ret;
    
    _mm_store_ss(&ret,V);
    
    return ret;
#endif

#ifdef BLASTER_RASTER_RCP_GENERIC
    return 1.0f/x;
#endif
}

static void put_fragment(bl_raster_t* raster,bl_fragment_t* fragment)
{
    raster->current_chunk->buffer[raster->current_chunk->count]=*fragment;
    raster->current_chunk->count++;
    if (raster->current_chunk->count==raster->current_chunk->size) {
        //printf("pushing a new chunk to render\n");
        bl_queue_push(raster->fragment_queue_out,raster->current_chunk);
        raster->current_chunk = bl_queue_pop(raster->fragment_queue_in);
    }
}

static void bl_raster_alloc_chunks(bl_raster_t* raster)
{
    for (int n=0;n<BL_MAX_CHUNKS;n++) {
        raster->fragment_chunk[n].size = BL_MAX_FRAGMENTS;
        raster->fragment_chunk[n].count = 0;
        raster->fragment_chunk[n].buffer = malloc(sizeof(bl_fragment_t)*BL_MAX_FRAGMENTS);
    }
}

static void bl_raster_free_chunks(bl_raster_t* raster)
{
    for (int n=0;n<BL_MAX_CHUNKS;n++) {
        raster->fragment_chunk[n].size = 0;
        raster->fragment_chunk[n].count = 0;
        
        free(raster->fragment_chunk[n].buffer);
        raster->fragment_chunk[n].buffer = 0;
    }
}

void bl_raster_update_chunk(bl_raster_t* raster,bl_fragment_chunk_t* chunk);
{
    for (size_t n=0;n<chunk->count;n++) {

        size_t offset=chunk->buffer[n].x+chunk->buffer[n].y*raster->screen_width;

        uint16_t* zbuffer=raster->depth_buffer->data;
        zbuffer+=offset;

        uint32_t* cbuffer=raster->color_buffer->data;
        cbuffer+=offset;

        if (chunk->buffer[n].depth<*zbuffer) {
            
            *zbuffer=chunk->buffer[n].depth;
            *cbuffer=chunk->buffer[n].pixel;
        }
    }
}

/*!
    Draw thread
*/
void draw_worker(void* arg)
{
    bl_raster_t* raster = (bl_raster_t*)arg;

    printf("Draw worker started!\n");
    
    while (1) {
        bl_command_t* cmd;
        
        cmd=bl_queue_pop(raster->queue_draw_commands);
        
        if (cmd==NULL) {
            break;
        }
        
        switch (cmd->type) {
        
            case BL_CMD_DRAW:
            
            break;
            
            default:
        }
    }
    
    printf("Draw worker is done\n");
}

/*!
    Update thread
*/
void update_worker(void* arg)
{
    bl_raster_t* raster = (bl_raster_t*)arg;

    printf("Update worker started!\n");
    
    
    while (1) {
        bl_command_t* cmd;
        
        cmd=bl_queue_pop(raster->queue_update_commands);
        
        if (cmd==NULL) {
            break;
        }
        
        switch (cmd->type) {
        
            case BL_CMD_CLEAR:
                bl_raster_clear_buffers(raster);
                bl_queue_push(raster->queue_free_commands,cmd);
            break;
            
            case BL_CMD_UPDATE:
                bl_raster_update_chunk(raster,cmd->chunk);
                
                bl_queue_push(raster->queue_free_chunks,cmd->chunk);
                bl_queue_push(raster->queue_free_commands,cmd);
            break;
            
            default:
                
        }
    }
    
    printf("update worker is done\n");
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
    
    raster->queue_free_chunks=bl_queue_new(BL_MAX_CHUNKS);
    
    for (size_t n=0;n<BL_MAX_CHUNKS;n++) {
        bl_queue_push(raster->queue_free_chunks,&raster->chunks[n]);
    }
    
    // create command queues
    raster->queue_update_commands=bl_queue_new(BL_MAX_COMMANDS);
    raster->queue_draw_commands=bl_queue_new(BL_MAX_COMMANDS);
    
    raster->queue_free_commands=bl_queue_new(BL_MAX_COMMANDS);
    
    // fill free commands
    for (int n=0;n<BL_MAX_COMMANDS;n++) {
        commands[n].type=BL_CMD_NONE;
        bl_queue_push(raster->queue_free_commands,&commands[n]);
    }
    
    pthread_create(&raster->thread_draw[0],NULL,draw_worker,raster);
    pthread_create(&raster->thread_draw[1],NULL,draw_worker,raster);
    pthread_create(&raster->thread_update,NULL,update_worker,raster);

    return raster;
}

void bl_raster_delete(bl_raster_t* raster)
{

    pthread_cancel(raster->thread_update);
    pthread_cancel(raster->thread_draw[0]);
    pthread_cancel(raster->thread_draw[1]);

    bl_queue_delete(raster->queue_free_chunks);
    bl_queue_delete(raster->queue_free_commands);
    bl_queue_delete(raster->queue_update_comands);
    bl_queue_delete(raster->queue_draw_commands);
    
    

    bl_texture_delete(raster->color_buffer);
    bl_texture_delete(raster->depth_buffer);
    
    bl_matrix_stack_delete(raster->modelview);
    bl_matrix_stack_delete(raster->projection);

    bl_raster_free_chunks(raster);
    
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

void bl_raster_clear(bl_raster_t* raster)
{
    bl_command_t* cmd;
    
    // obtain a free command
    cmd = bl_queue_pop(raster->queue_free_commands);
    
    cmd->type=BL_CMD_CLEAR;
    
    // push to update thread
    bl_queue_push(raster->queue_update_worker,cmd);
}

#ifdef BLASTER_RASTER_CLEAR_SSE2
void bl_raster_clear_buffers(bl_raster_t* raster)
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

}
#endif

#ifdef BLASTER_RASTER_CLEAR_GENERIC
void bl_raster_clear_buffers(bl_raster_t* raster)
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

}
#endif

void bl_raster_update(bl_raster_t* raster)
{

    //printf("sync...");
    bl_queue_push(raster->fragment_queue_out,raster->current_chunk);
    raster->current_chunk = bl_queue_pop(raster->fragment_queue_in);
    
    while (!bl_queue_is_empty(raster->fragment_queue_out)) {
        //printf("%d\n",raster->fragment_queue_out->size);
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

void bl_raster_draw(bl_raster_t* raster,bl_vbo_t* vbo,uint8_t type)
{
    bl_command_t* cmd;
    
    cmd = bl_queue_pop(raster->cmd_queue_empty);
    
    cmd->type = BL_CMD_DRAW;
    
    bl_queue_push(raster->cmd_queue_out);
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
        
        bl_fragment_t fragment;
        fragment.pixel=bl_color_get_pixel(&source[n].color).value;
        fragment.x=win[0];
        fragment.y=win[1];
        fragment.depth=win[2];
        put_fragment(raster,&fragment);
        
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
    
    static int max_dx=0;
    static int max_dy=0;
    
    bl_matrix_t matrix;
    
    // precompute modelview and projection matrix
    bl_matrix_mult(&matrix,raster->projection->matrix,raster->modelview->matrix);
    
    bl_vector_t clip[2];
    bl_vector_t ndc[2];
    
    bl_vector_t wl;
    
    wl.x=raster->screen_width*0.5f;
    wl.y=raster->screen_height*0.5f;
    wl.z=65535*0.5f;
    wl.w=0.0f;
    
    
    uint32_t win[2][4];
    
    bl_fragment_t fragment;
    
    for (int n=0;n<vbo->size;n+=2) {
        bl_vector_mult(&clip[0],&source[n].pos,&matrix);
        bl_vector_mult(&clip[1],&source[n+1].pos,&matrix);
        //bl_vector_mult_dual(&clip[0],&source[n].pos,&clip[1],&source[n+1].pos,&matrix);
        
        //float w=1.0f/clip[0].w;
        float w=rcp(clip[0].w);
        
        ndc[0].x=clip[0].x*w;
        ndc[0].y=clip[0].y*w;
        ndc[0].z=clip[0].z*w;
        ndc[0].w=clip[0].w*w;
        
        //w=1.0f/clip[1].w;
        w=rcp(clip[1].w);
        
        ndc[1].x=clip[1].x*w;
        ndc[1].y=clip[1].y*w;
        ndc[1].z=clip[1].z*w;
        ndc[1].w=clip[1].w*w;
        
        //HACK, lines should be tesselated
        if (ndc[0].z<-1.0f || ndc[0].z>1.0f || ndc[1].z<-1.0f || ndc[1].z>1.0f) {
            continue;
        }
        
        win[0][0]=(ndc[0].x*wl.x)+wl.x;
        win[0][1]=(ndc[0].y*wl.y)+wl.y;
        win[0][2]=(ndc[0].z*wl.z)+wl.z;
        
        win[1][0]=(ndc[1].x*wl.x)+wl.x;
        win[1][1]=(ndc[1].y*wl.y)+wl.y;
        win[1][2]=(ndc[1].z*wl.z)+wl.z;
        
        //float rz1 =1.0f/ndc[0].z;
        //float rz2 =1.0f/ndc[1].z;
        //float rz1=rcp(ndc[0].z);
        //float rz2=rcp(ndc[1].z);
        
        int x0 = win[0][0];
        int y0 = win[0][1];
        
        int x1 = win[1][0];
        int y1 = win[1][1];
        
        int xlimit=raster->screen_width-1;
        int ylimit=raster->screen_height-1;
        
        const int dx = x1-x0;
        const int dy = y1-y0;
        
        
        const int incx = (dx<0) ? -1 : 1;
        const int incy = (dy<0) ? -1 : 1;
        
        const int step = abs(dy)-abs(dx);
        
        int error;
        int ey;
        int ex;
        
        ey = abs(dy);
        ex = abs(dx);
        fragment.pixel=bl_color_get_pixel(&source[n].color).value;
        
        // horitzontal ladder
        if (step<0) {
            
            
            error = 2*ey - ex;
            
            fragment.y=y0;
            
            for (fragment.x=x0;fragment.x!=x1;fragment.x+=incx) {
                if (fragment.x>0 && fragment.x<xlimit &&
                    fragment.y>0 && fragment.y<ylimit) {
                    
                    put_fragment(raster,&fragment);
                
                }
                
                
                if (error > 0) {
                    fragment.y += incy;
                    error -= 2*ex;
                }
                error += 2*ey;
            }
        }
        else {
            // vertical ladder
            
            error = 2*ex - ey;
            
            fragment.x=x0;
            
            for (fragment.y=y0;fragment.y!=y1;fragment.y+=incy) {
                if (fragment.x>0 && fragment.x<xlimit &&
                    fragment.y>0 && fragment.y<ylimit) {
                    
                    put_fragment(raster,&fragment);
                
                }
                
                if (error>0) {
                    fragment.x += incx;
                    error -= 2*ey;
                }
                error += 2*ex;
            }
        }
        
        
    }
}

void bl_raster_draw_triangles(bl_raster_t* raster, bl_vbo_t* vbo)
{
    typedef struct {
        bl_vector_t pos;
        bl_color_t color;
    } vertex_t;
    
    typedef struct {
        uint32_t x;
        uint32_t y;
        uint32_t z;
        uint32_t w;
    } window_vertex_t;
    
    vertex_t* source = (vertex_t*) vbo->data;
    
    bl_matrix_t matrix;

    bl_vector_t clip[3];
    bl_vector_t ndc[3];
    
    bl_vector_t wl;
    
    wl.x=raster->screen_width/2.0f;
    wl.y=raster->screen_height/2.0f;
    wl.z=65535/2.0f;
    wl.w=0.0f;
    
    window_vertex_t wv[3];
    
    // precompute modelview and projection matrix
    bl_matrix_mult(&matrix,raster->projection->matrix,raster->modelview->matrix);
    
    for (int n=0;n<vbo->size;n+=3) {
        bl_vector_mult(&clip[0],&source[n].pos,&matrix);
        bl_vector_mult(&clip[1],&source[n].pos,&matrix);
        bl_vector_mult(&clip[2],&source[n].pos,&matrix);
        
        float w;
        
        for (int i=0;i<3;i++) {
            w=1.0f/clip[i].w;
            
            ndc[i].x=clip[i].x*w;
            ndc[i].y=clip[i].y*w;
            ndc[i].z=clip[i].z*w;
            ndc[i].w=clip[i].w*w;
        }
        
        //tessellation
        //TODO: do it right
        int out=0;
        
        for (int i=0;i<3;i++) {
            if (ndc[i].z<-1.0f || ndc[i].z>1.0f) {
                out=1;
            }
        }
        
        if (out) {
            continue;
        }
        
        for (int i=0;i<3;i++) {
            wv[i].x=(ndc[i].x*wl.x)+wl.x;
            wv[i].y=(ndc[i].y*wl.y)+wl.y;
            wv[i].z=(ndc[i].z*wl.z)+wl.z;
        }
        
        uint32_t xmin = MIN(wv[0].x,wv[1].x);
        xmin = MAX(xmin,wv[2].x);
        
        if (xmin>=raster->screen_width) {
            continue;
        }
        
        uint32_t xmax = MAX(wv[0].x,wv[1].x);
        xmax = MAX(xmax,wv[2].x);
        
        if (xmax<0) {
            continue;
        }
        
        uint32_t ymin = MIN(wv[0].y,wv[1].y);
        ymin = MAX(ymin,wv[2].y);
        
        if (ymin>=raster->screen_height) {
            continue;
        }
        
        uint32_t ymax = MAX(wv[0].y,wv[1].y);
        ymax = MAX(ymax,wv[2].y);
        
        if (ymax<0) {
            continue;
        }
        
        xmin = MAX(0,xmin);
        xmax = MIN(raster->screen_width,xmax);
        
        ymin = MAX(0,ymin);
        ymax = MIN(raster->screen_height,ymax);
    }
}
