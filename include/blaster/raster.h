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

#ifndef BLASTER_RASTER
#define BLASTER_RASTER

#ifdef __cplusplus
extern "C" {
#endif

#include "constants.h"
#include "texture.h"
#include "matrix.h"
#include "matrix_stack.h"
#include "vbo.h"
#include "queue.h"

#include <pthread.h>

#define BL_MAX_CHUNKS       128
#define BL_MAX_FRAGMENTS    32767
#define BL_MAX_COMMANDS     128

typedef struct bl_fragment_u {
    int16_t x;
    int16_t y;
    uint16_t depth;
    uint32_t pixel;
    //uint16_t padding[9];
} bl_fragment_t;

typedef struct bl_fragment_chunk_u {
    size_t size;
    size_t count;
    bl_fragment_t* buffer;
} bl_fragment_chunk_t;

typedef struct bl_command_u {

    /*! command type */
    uint8_t type;
    
    /*! status */
    uint8_t status;
    
    union {
        struct {
            uint32_t color;
        } clear;
        
        struct {
            bl_vbo_t* vbo;
            uint8_t type;
            size_t start;
            size_t count;
            
        } draw;
        
        struct {
            bl_fragment_chunk_t* chunk;
        } update;
    };
    
} bl_command_t;

/*!
    Raster main structure
*/
typedef struct {

    /*! screen width in pixels */
    int screen_width;

    /*! screen height in pixels */
    int screen_height;

    /*! texture for color buffer */
    bl_texture_t* color_buffer;
    
    /*! 16 bit texture for depth buffer */
    bl_texture_t* depth_buffer;
    
    /*! modelview matrix */
    bl_matrix_stack_t* modelview;
    
    /*! projection matrix */
    bl_matrix_stack_t* projection;
    
    /*! color used for clear */
    bl_color_t clear_color;
    
    
    bl_fragment_chunk_t chunks[BL_MAX_CHUNKS];
    bl_queue_t* queue_free_chunks;
    
    bl_command_t draw_commands[BL_MAX_COMMANDS];
    bl_command_t update_commands[BL_MAX_COMMANDS];
    
    bl_queue_t* queue_free_draw_commands;
    bl_queue_t* queue_free_update_commands;
    
    bl_queue_t* queue_update_commands;
    bl_queue_t* queue_draw_commands;
    
    int draw_workers;
    int update_workers;
    
    pthread_t* thread_draw;
    pthread_t* thread_update;
    
} bl_raster_t;

/*!
    Create a new raster
*/
bl_raster_t* bl_raster_new(int width,int height,int draw_workers,int update_workers);

/*!
    Free raster
*/
void bl_raster_delete(bl_raster_t* raster);

/*!
    Resize framebuffer
*/
void bl_raster_resize(bl_raster_t* raster,int width,int height);

/*!
    Sets clear pixel color
*/
void bl_raster_set_clear_color(bl_raster_t* raster,bl_color_t* color);

/*!
    Clear buffers
*/
void bl_raster_clear(bl_raster_t* raster);

/*!
    Update (flush all commands and render result)
*/
void bl_raster_update(bl_raster_t* raster);

/*!
    Wait for all draw commands to complete
*/
void bl_raster_flush_draw(bl_raster_t* raster);

/*!
    Wait for all update commands to complete
*/
void bl_raster_flush_update(bl_raster_t* raster);

/*!
    Get width in pixels
*/
int bl_raster_get_width(bl_raster_t* raster);

/*!
    Get height in pixels
*/
int bl_raster_get_height(bl_raster_t* raster);

/*!
    Draws a vertex buffer (user-side function)
*/
void bl_raster_draw(bl_raster_t* raster,bl_vbo_t* vbo,uint8_t type);

#ifdef __cplusplus
}
#endif

#endif

