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

#include <stdio.h>
#include <stdlib.h>


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
    uint32_t pixel=bl_color_get_pixel(raster->clear_color);
    
    uint32_t* color = raster->color_buffer->data;
    uint16_t* depth = raster->depth_buffer->data;
    
    for (int j=0;j<raster->screen_height;j++) {
        for (int i=0;i<raster->screen_width;i++) {
            color[i+j*raster->screen_width]=pixel;
            depth[i+j*raster->screen_width]=0xFFFF;
        }
    }
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

