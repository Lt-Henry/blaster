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
#include <blaster/time.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

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
void bl_raster_draw_points(bl_raster_t* raster, bl_vbo_t* vbo, size_t start,size_t count);

/*!
    Draws a vertex bufferas lines
*/
void bl_raster_draw_lines(bl_raster_t* raster, bl_vbo_t* vbo, size_t start,size_t count);

/*!
    Draws a vertex buffer as triangles
*/
void bl_raster_draw_triangles(bl_raster_t* raster, bl_vbo_t* vbo, size_t start,size_t count);

/*!
    Push fragments to frame buffer
*/
void bl_raster_update_chunk(bl_raster_t* raster,bl_fragment_chunk_t* chunk);

/*!
    default shader
*/
void bl_raster_basic_vertex_shader(bl_raster_t* raster, size_t n, float* attributes, float* variying, float* flat, bl_vector_t* position);
void bl_raster_basic_fragment_shader(bl_raster_t* raster, float* attributes, float* variying, float* flat, bl_fragment_t* fragment);

static uint32_t max_u32(uint32_t a,uint32_t b)
{
    if (a>b) {
        return a;
    }
    else {
        return b;
    }
}

static uint32_t min_u32(uint32_t a,uint32_t b)
{
    if (a<b) {
        return a;
    }
    else {
        return b;
    }
}

static void swap_u32(uint32_t* a,uint32_t* b)
{
    uint32_t tmp = *a;
    *a=*b;
    *b=tmp;
}

static float max_f32(float a,float b)
{
    if (a>b) {
        return a;
    }
    else {
        return b;
    }
}

static float min_f32(float a,float b)
{
    if (a<b) {
        return a;
    }
    else {
        return b;
    }
}

#ifdef BLASTER_RASTER_RCP_SSE
static float rcp(float x)
{
    __m128 V;
    
    V=_mm_set_ss(x);
    V=_mm_rcp_ss(V);
    
    float ret;
    
    _mm_store_ss(&ret,V);
    
    return ret;
}
#endif

#ifdef BLASTER_RASTER_RCP_GENERIC
static float rcp(float x)
{
    return 1.0f/x;
}
#endif

static void bl_raster_put_fragment(bl_raster_t* raster,bl_fragment_chunk_t** chunk,bl_fragment_t* fragment)
{
    (*chunk)->buffer[(*chunk)->count]=*fragment;
    (*chunk)->count++;
    
    if ((*chunk)->count==(*chunk)->size) {
        bl_command_t* cmd = bl_queue_pop(raster->queue_free_update_commands);
        cmd->type = BL_CMD_UPDATE;
        cmd->update.chunk=*chunk;
        bl_queue_push(raster->queue_update_commands,cmd);
        *chunk = bl_queue_pop(raster->queue_free_chunks);
    }
}

static void bl_raster_commit_chunk(bl_raster_t* raster,
bl_fragment_chunk_t** chunk)
{
    bl_command_t* cmd = bl_queue_pop(raster->queue_free_update_commands);
    cmd->type = BL_CMD_UPDATE;
    cmd->update.chunk=*chunk;
    
    bl_queue_push(raster->queue_update_commands,cmd);
    
    *chunk=NULL;
}

static void bl_raster_alloc_chunks(bl_raster_t* raster)
{
    for (size_t n=0;n<BL_MAX_CHUNKS;n++) {
        raster->chunks[n].size = BL_MAX_FRAGMENTS;
        raster->chunks[n].count = 0;
        raster->chunks[n].buffer = malloc(sizeof(bl_fragment_t)*BL_MAX_FRAGMENTS);
    }
}

static void bl_raster_free_chunks(bl_raster_t* raster)
{
    for (size_t n=0;n<BL_MAX_CHUNKS;n++) {
        raster->chunks[n].size = 0;
        raster->chunks[n].count = 0;
        
        free(raster->chunks[n].buffer);
        raster->chunks[n].buffer = 0;
    }
}

void bl_raster_update_chunk(bl_raster_t* raster,bl_fragment_chunk_t* chunk)
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
    
    chunk->count=0;
}

/*!
    Draw thread
*/
void* draw_worker(void* args)
{
    bl_worker_t* worker = (bl_worker_t*)args;
    bl_raster_t* raster = (bl_raster_t*)worker->args;

    printf("Draw worker started!\n");
    
    uint64_t t1,t2,t3;
    
    while (1) {
        bl_command_t* cmd;
        
        t1=bl_time_us();
        cmd=bl_queue_pop(raster->queue_draw_commands);
        t2=bl_time_us();
        
        if (worker->time.start==0) {
            worker->time.start=t2;
            worker->time.wait=0;
            worker->time.work=0;
            worker->time.last=0;
        }
        if (worker->time.last!=0) {
            worker->time.wait+=t2-t1;
        }
        
        if (cmd==NULL) {
            break;
        }
        
        //printf("draw cmd: %d\n",cmd->type);
        
        switch (cmd->type) {
        
            case BL_CMD_DRAW:
                switch (cmd->draw.type) {
                    
                    case BL_VBO_POINTS:
                        bl_raster_draw_points(raster,cmd->draw.vbo,cmd->draw.start,cmd->draw.count);
                    break;
                    
                    case BL_VBO_LINES:
                        bl_raster_draw_lines(raster,cmd->draw.vbo,cmd->draw.start,cmd->draw.count);
                    break;
                    
                    case BL_VBO_TRIANGLES:
                        bl_raster_draw_triangles(raster,cmd->draw.vbo,cmd->draw.start,cmd->draw.count);
                    break;
                }
            break;
            
        }
        
        bl_queue_push(raster->queue_free_draw_commands,cmd);
        //printf("draw cmd done\n");
        t3=bl_time_us();
        worker->time.work+=t3-t2;

        worker->time.last=t3;
    }
    
    printf("Draw worker is done\n");
    
    return NULL;
}

/*!
    Update thread
*/
void* update_worker(void* args)
{
    bl_worker_t* worker = (bl_worker_t*)args;
    bl_raster_t* raster = (bl_raster_t*)worker->args;


    printf("Update worker started!\n");
    
    uint64_t t1,t2,t3;
    
    while (1) {
        bl_command_t* cmd;
        
        t1=bl_time_us();
        cmd=bl_queue_pop(raster->queue_update_commands);
        t2=bl_time_us();
        
        if (worker->time.start==0) {
            worker->time.start=t2;
            worker->time.wait=0;
            worker->time.work=0;
            worker->time.last=0;
        }
        
        if (worker->time.last!=0) {
            worker->time.wait+=t2-t1;
        }
        
        if (cmd==NULL) {
            break;
        }
        //printf("update cmd: %d\n",cmd->type);
        
        switch (cmd->type) {
        
            case BL_CMD_CLEAR:
                
                bl_raster_clear_buffers(raster);
                bl_queue_push(raster->queue_free_update_commands,cmd);
            break;
            
            case BL_CMD_UPDATE:
                bl_raster_update_chunk(raster,cmd->update.chunk);
                bl_queue_push(raster->queue_free_chunks,cmd->update.chunk);
                bl_queue_push(raster->queue_free_update_commands,cmd);
            break;
            
        }
        
        t3=bl_time_us();
        worker->time.work+=t3-t2;
        worker->time.last=t3;
        //printf("update cmd done\n");
    }
    
    printf("update worker is done\n");
    
    return NULL;
}

bl_raster_t* bl_raster_new(int width,int height,int draw_workers,int update_workers)
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
    
    bl_raster_alloc_chunks(raster);
    
    raster->queue_free_chunks=bl_queue_new(BL_MAX_CHUNKS);
    
    for (size_t n=0;n<BL_MAX_CHUNKS;n++) {
        bl_queue_push(raster->queue_free_chunks,&raster->chunks[n]);
    }
    
    // create command queues
    raster->queue_update_commands=bl_queue_new(BL_MAX_COMMANDS);
    raster->queue_draw_commands=bl_queue_new(BL_MAX_COMMANDS);
    
    raster->queue_free_draw_commands=bl_queue_new(BL_MAX_COMMANDS);
    raster->queue_free_update_commands=bl_queue_new(BL_MAX_COMMANDS);
    
    // fill free commands
    for (int n=0;n<BL_MAX_COMMANDS;n++) {
        raster->draw_commands[n].type=BL_CMD_NONE;
        bl_queue_push(raster->queue_free_draw_commands,&raster->draw_commands[n]);
        
        raster->update_commands[n].type=BL_CMD_NONE;
        bl_queue_push(raster->queue_free_update_commands,&raster->update_commands[n]);
    }
    
    raster->draw_workers=draw_workers;
    raster->update_workers=update_workers;
    
    raster->workers=malloc((draw_workers+update_workers)*sizeof(bl_worker_t*));
    
    int w=0;
    for (int n=0;n<draw_workers;n++,w++) {
        raster->workers[w]=bl_worker_new(BL_WORKER_DRAW,0);
        bl_worker_start(raster->workers[w],draw_worker,raster);
    }
    for (int n=0;n<update_workers;n++,w++) {
        raster->workers[w]=bl_worker_new(BL_WORKER_UPDATE,0);
        bl_worker_start(raster->workers[w],update_worker,raster);
    }
    
    return raster;
}

void bl_raster_delete(bl_raster_t* raster)
{
    int num_workers=raster->draw_workers+raster->update_workers;
    
    for (int n=0;n<num_workers;n++) {
        bl_worker_stop(raster->workers[n]);
        bl_worker_delete(raster->workers[n]);
    }
    
    free(raster->workers);
    
    bl_queue_delete(raster->queue_free_chunks);
    bl_queue_delete(raster->queue_free_draw_commands);
    bl_queue_delete(raster->queue_free_update_commands);
    bl_queue_delete(raster->queue_update_commands);
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
    for (size_t n=0;n<raster->draw_workers+raster->update_workers;n++) {
        raster->workers[n]->time.start=0;
    }
    
    bl_command_t* cmd;
    
    //printf("clear buffers...\n");
    
    // obtain a free command
    cmd = bl_queue_pop(raster->queue_free_update_commands);
    
    cmd->type=BL_CMD_CLEAR;
    
    // push to update thread
    bl_queue_push(raster->queue_update_commands,cmd);
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
    
    for (size_t j=0;j<raster->screen_height;j++) {
        for (size_t i=0;i<raster->screen_width;i++) {
            color[i+j*raster->screen_width]=pixel.value;
            depth[i+j*raster->screen_width]=0xFFFF;
        }
    }
}
#endif

void bl_raster_update(bl_raster_t* raster)
{

}

void bl_raster_flush_draw(bl_raster_t* raster)
{
    while (!bl_queue_is_full(raster->queue_free_draw_commands)) {
        usleep(250);
    }
}

void bl_raster_flush_update(bl_raster_t* raster)
{
    while (!bl_queue_is_full(raster->queue_free_update_commands)) {
        usleep(250);
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
    
    int nv=1;
    size_t block=1024;
    size_t count;
    
    size_t start=0;
    size_t end;
    
    switch (type) {

        case BL_VBO_LINES:
            nv=2;
        break;
        
        case BL_VBO_TRIANGLES:
            nv=3;
        break;
    }
    
    count=vbo->size/nv;
    size_t num=0;
    
    while (start<count) {
        
        cmd = bl_queue_pop(raster->queue_free_draw_commands);
        
        num=(start*nv)+(block*nv);
        num=min_u32(num,vbo->size);
        num=num-(start*nv);
        
        cmd->type = BL_CMD_DRAW;
        cmd->draw.type=type;
        cmd->draw.vbo=vbo;
        cmd->draw.start=start*nv;
        cmd->draw.count=num;
        //printf("pushing a draw job [%ld,%ld]\n",start*nv,num);
        bl_queue_push(raster->queue_draw_commands,cmd);
        
        start+=block;
    }
}

void bl_raster_set_texture(bl_raster_t* raster,bl_texture_t* texture)
{
    raster->texture=texture;
}

void bl_raster_draw_points(bl_raster_t* raster,bl_vbo_t* vbo,size_t start,size_t count)
{

    bl_fragment_chunk_t* chunk;
    
    //get a free chunk
    chunk = bl_queue_pop(raster->queue_free_chunks);

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
    
    for (size_t n=0;n<count;n++) {
        
        //clip coordinates
        bl_vector_mult(&clip,&source[start+n].pos,&matrix);
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
        bl_raster_put_fragment(raster,&chunk,&fragment);
        
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
    
    bl_raster_commit_chunk(raster,&chunk);
}

void bl_raster_draw_lines(bl_raster_t* raster, bl_vbo_t* vbo,size_t start,size_t count)
{
    typedef struct {
        bl_vector_t pos;
        bl_color_t color;
    } point_t;
    
    bl_fragment_chunk_t* chunk;
    
    //get a free chunk
    chunk = bl_queue_pop(raster->queue_free_chunks);


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
    
    for (size_t n=0;n<count;n+=2) {
        bl_vector_mult(&clip[0],&source[start+n].pos,&matrix);
        bl_vector_mult(&clip[1],&source[start+n+1].pos,&matrix);
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
                    
                    bl_raster_put_fragment(raster,&chunk,&fragment);
                
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
                    
                    bl_raster_put_fragment(raster,&chunk,&fragment);
                
                }
                
                if (error>0) {
                    fragment.x += incx;
                    error -= 2*ey;
                }
                error += 2*ex;
            }
        }
        
    }
    
    bl_raster_commit_chunk(raster,&chunk);
}

static int32_t orient(int32_t x0, int32_t y0, int32_t x1,int32_t y1, int32_t x2, int32_t y2)
{
    return (x1-x0)*(y2-y0) - (y1-y0)*(x2-x0);
}

static float orientf(float x0, float y0, float x1,float y1, float x2, float y2)
{
    return (x1-x0)*(y2-y0) - (y1-y0)*(x2-x0);
}

void bl_raster_draw_triangles(bl_raster_t* raster, bl_vbo_t* vbo,size_t start,size_t count)
{
    float variying[48];
    float mix[16];
    float flat[16];
    size_t var_count = 0;
    
    typedef struct {
        bl_vector_t pos;
        bl_vector_t normal;
        bl_color_t color;
    } vertex_t;
    /*
    typedef struct {
        uint32_t x;
        uint32_t y;
        uint32_t z;
        uint32_t w;
    } window_vertex_t;
    */
    typedef struct {
        float x;
        float y;
        float z;
        float w;
    } window_vertex_t;
    
    bl_fragment_chunk_t* chunk;
    
    //get a free chunk
    chunk = bl_queue_pop(raster->queue_free_chunks);
    
    vertex_t* source = (vertex_t*) vbo->data;
    
    bl_matrix_t matrix;

    bl_vector_t clip[3];
    bl_vector_t ndc[3];
    bl_vector_t normal[3];
    bl_vector_t face_normal;
    bl_vector_t ab;
    bl_vector_t ac;
    
    bl_vector_t wl;
    
    wl.x=raster->screen_width/2.0f;
    wl.y=raster->screen_height/2.0f;
    wl.z=65535/2.0f;
    wl.w=0.0f;
    
    window_vertex_t wv[3];
    
    float rz[3];
    float rvalues[48];
    float values[12];
    
    // precompute modelview and projection matrix
    //bl_matrix_mult(&matrix,raster->projection->matrix,raster->modelview->matrix);
    
    for (size_t n=0;n<count;n+=3) {
        
        for (size_t v=0;v<3;v++) {
            //bl_raster_basic_vertex_shader(&shader,start+n+v,v,&matrix,&clip[v]);
            bl_raster_basic_vertex_shader(raster,v,
                                          bl_vbo_get(vbo,start+n+v),
                                          variying + (16*v),
                                          flat,
                                          &clip[v]);
        }
    
        //bl_vector_mult(&normal[0],&source[start+n].normal,&matrix);
        /*
        bl_vector_sub(&ab,&source[start+n+1].pos,&source[start+n].pos);
        bl_vector_sub(&ac,&source[start+n+2].pos,&source[start+n].pos);
        bl_vector_cross(&face_normal,&ab,&ac);
        bl_vector_mult(&face_normal,&face_normal,&matrix);
        bl_vector_normalize(&face_normal);
        */
        float w;
        
        for (int i=0;i<3;i++) {
            w=1.0f/clip[i].w;
            
            ndc[i].x=clip[i].x*w;
            ndc[i].y=clip[i].y*w;
            ndc[i].z=clip[i].z*w;
            ndc[i].w=clip[i].w*w;
        }
        
        // full clip
        int vout[3]={0,0,0};
        
        for (int i=0;i<3;i++) {
            for (int j=0;j<3;j++) {
                if (ndc[i].data[j]<=-1.0f) {
                    vout[j]=vout[j] | (1<<i);
                }
                
                if (ndc[i].data[j]>=1.0f) {
                    vout[j]=vout[j] | (1<<(3+i));
                }
            }
        }
        
        // z clip, for now, only passing triangles fully inside the clip space
        if (vout[2]!=0) {
            continue;
        }
        
        // all x are either left or right from clip space
        if (vout[0]==7 || vout[0]==56) {
            continue;
        }
        
        // all y are either left or right from clip space
        if (vout[1]==7 || vout[1]==56) {
            continue;
        }
        
        //tessellation
        //TODO: do it right
        
        for (int i=0;i<3;i++) {
            wv[i].x=(ndc[i].x*wl.x)+wl.x;
            wv[i].y=(ndc[i].y*wl.y)+wl.y;
            wv[i].z=(ndc[i].z*wl.z)+wl.z;
        }
        
        float xmin = min_f32(wv[0].x,wv[1].x);
        xmin = min_f32(xmin,wv[2].x);
        /*
        if (xmin>raster->screen_width) {
            printf("vout %d %d %d\n",vout[0],vout[1],vout[2]);
            continue;
        }
        */
        float xmax = max_f32(wv[0].x,wv[1].x);
        xmax = max_f32(xmax,wv[2].x);
        /*
        if (xmax<0) {
            continue;
        }
        */
        float ymin = min_f32(wv[0].y,wv[1].y);
        ymin = min_f32(ymin,wv[2].y);
        /*
        if (ymin>=raster->screen_height) {
            continue;
        }
        */
        float ymax = max_f32(wv[0].y,wv[1].y);
        ymax = max_f32(ymax,wv[2].y);
        /*
        if (ymax<0) {
            continue;
        }
        */
        xmin = max_f32(0,xmin);
        xmax = min_f32(raster->screen_width,xmax);
        
        ymin = max_f32(0,ymin);
        ymax = min_f32(raster->screen_height,ymax);
        
        bl_fragment_t fragment;
        /*
        rz[0]=1.0f/ndc[0].z;
        rz[1]=1.0f/ndc[1].z;
        rz[2]=1.0f/ndc[2].z;
        */
        rz[0]=rcp(ndc[0].z);
        rz[1]=rcp(ndc[1].z);
        rz[2]=rcp(ndc[2].z);
        /*
        for (size_t v=0;v<3;v++) {
            float* tmp = bl_vbo_get(vbo,start+n+v);
            for (size_t w=4;w<vbo->vertex_size;w++) {
                rvalues[(vbo->vertex_size*v)+w] = tmp[w]/ndc[v].z;
            }
        }
        */
        float lambda[4];
        
        float area = 1.0f/orientf(wv[0].x,wv[0].y,wv[1].x,wv[1].y,wv[2].x,wv[2].y);
        
        float z;
        
        int32_t sx = xmin;
        int32_t ex = xmax+1.0f;
        int32_t sy = ymin;
        int32_t ey = ymax+1.0f;
     
        //bl_color_t color = light_diffuse;
        /*
        bl_color_t color = base_color[n%4];
        bl_color_scale(&color,cosAlpha);
        bl_color_add(&color,&light_ambient,&color);
        bl_color_clamp(&color);
        fragment.pixel=bl_color_get_pixel(&color).value;
        */
        /*
        float A=area*(rz[1]-rz[0]);
        float B=area*(rz[2]-rz[0]);
        */
        
        float A=area*(ndc[1].z-ndc[0].z);
        float B=area*(ndc[2].z-ndc[0].z);
        
        int in;
        
        float a = (wv[2].x-wv[1].x);
        float b = (wv[2].y-wv[1].y);
        float c = (wv[0].x-wv[2].x);
        float d = (wv[0].y-wv[2].y);
        float e = (wv[1].x-wv[0].x);
        float f = (wv[1].y-wv[0].y);
        
        float pl[4];
        
        for (int32_t y=sy;y!=ey;y++) {
            in = false;
            
            pl[0] = a*(y-wv[1].y);
            pl[1] = c*(y-wv[2].y);
            pl[2] = e*(y-wv[0].y);
            
            for (int32_t x=sx;x!=ex;x++) {
                
                lambda[0]=orientf(wv[1].x,wv[1].y,wv[2].x,wv[2].y,x,y);
                lambda[1]=orientf(wv[2].x,wv[2].y,wv[0].x,wv[0].y,x,y);
                lambda[2]=orientf(wv[0].x,wv[0].y,wv[1].x,wv[1].y,x,y);
                
                /*
                lambda[0] = pl[0] - b*(x-wv[1].x);
                lambda[1] = pl[1] - d*(x-wv[2].x);
                lambda[2] = pl[2] - f*(x-wv[0].x);
                */
                if (lambda[0]<0 && lambda[1]<0 && lambda[2]<0) {
                    in = true;
                    fragment.x=x;
                    fragment.y=y;
                    //fragment.pixel=bl_color_get_pixel(&color).value;
                    
                    lambda[0]=lambda[0]*area;
                    lambda[1]=lambda[1]*area;
                    lambda[2]=lambda[2]*area;
                    
                    //z = rz[0]*lambda[0] + rz[1]*lambda[1] + rz[2]*lambda[2];
                   //z = rz[0]+lambda[1]*A + lambda[2]*B;
                    z = ndc[0].z*lambda[0] + ndc[1].z*lambda[1] + ndc[2].z*lambda[2];
                    
                    //z = ndc[0].z+lambda[1]*A + lambda[2]*B;
                    
                    fragment.depth=(z*wl.z)+wl.z;
                    
                    /*
                    for (size_t w=4;w<vbo->vertex_size;w++) {
                        size_t q = vbo->vertex_size;
                        values[w] = rvalues[q+w]*lambda[0] + rvalues[(q*2)+w]*lambda[1] + rvalues[(q*3)+w]*lambda[2];
                    }
                    */
                    
                    bl_raster_basic_fragment_shader(raster,
                                                    NULL,
                                                    mix,
                                                    flat,
                                                    &fragment);
                    
                    //bl_color_t dc;
                    //bl_color_set(&dc,(z*0.5f)+1.0f,(z*0.5f)+1.0f,(z*0.5f)+1.0f,1.0f);
                    //fragment.pixel=bl_color_get_pixel(&dc).value;
                    
                    bl_raster_put_fragment(raster,&chunk,&fragment);
                }
                else {
                    if (in) {
                        break;
                    }
                }
            }
        }
    }
    
    bl_raster_commit_chunk(raster,&chunk);
}

void bl_raster_basic_vertex_shader(bl_raster_t* raster, size_t n, float* attributes, float* variying, float* flat, bl_vector_t* position)
{
    bl_matrix_t* matrix = (bl_matrix_t*) raster->uniform;
    bl_vector_t* vector = (bl_vector_t*) attributes;
    
    bl_vector_mult(position,vector,matrix); //postion * mvp matrix
    
    if (n==0) {
        bl_vector_t light_pos = {0.0f,1.0f,-1.0f,0.0f};
        bl_color_t light_diffuse = {0.5f,0.5f,0.5f,1.0f};
        
        bl_vector_t normal;
        bl_vector_mult(&normal,&vector[1],&matrix[1]); //normal * mv matrix
        
        float cosAlpha = bl_vector_dot(&normal,&light_pos);
        cosAlpha=fabs(cosAlpha);
        
        bl_color_t* material = (bl_color_t*) flat;
        bl_color_set(material,0.5f,0.5f,0.5f,1.0f);
        
        bl_color_scale(material,cosAlpha);
        
    }
}

void bl_raster_basic_fragment_shader(bl_raster_t* raster, float* attributes, float* variying, float* flat, bl_fragment_t* fragment)
{
    bl_color_t* color = (bl_color_t*) flat;
    fragment->pixel=bl_color_get_pixel(color).value;
    //fragment->pixel=0xff000000;
}
