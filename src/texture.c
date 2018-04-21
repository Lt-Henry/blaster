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

#include <blaster/constants.h>
#include <blaster/texture.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static uint8_t bl_texture_get_bpp(uint8_t type)
{
    switch (type) {
        case BL_TEXTURE_U8:
            return 1;
        break;
        
        case BL_TEXTURE_U16:
            return 2;
        break;
        
        case BL_TEXTURE_U32:
        case BL_TEXTURE_F32:
            return 4;
        break;
        
        case BL_TEXTURE_F64:
            return 8;
        break;
        
        default:
            return 0; //bad thing may happen
    }
}

bl_texture_t* bl_texture_new(int width,int height,uint8_t type)
{
    bl_texture_t* texture = NULL;

    texture = malloc(sizeof(bl_texture_t));

    texture->type=type;
    texture->bpp=bl_texture_get_bpp(type);
    
    texture->data=aligned_alloc(16,texture->bpp*width*height);
    texture->width=width;
    texture->height=height;

    return texture;
}

bl_texture_t* bl_texture_new_from_data(int width,int height,uint8_t type,const void* data)
{
    bl_texture_t* texture = bl_texture_new(width,height,type);
    memcpy((void*)texture->data,data,texture->bpp*width*height);

    return texture;
}

void bl_texture_delete(bl_texture_t* t)
{
    free(t->data);
    free(t);
}

void bl_texture_set_pixel(bl_texture_t* t,int x,int y,bl_pixel_t pixel)
{
    uint32_t* ptr=t->data;
    ptr[x+y*t->width]=pixel.value;
}

bl_pixel_t bl_texture_get_pixel(bl_texture_t* t,int x,int y)
{
    uint32_t* ptr=t->data;
    return (bl_pixel_t)ptr[x+y*t->width];
}
