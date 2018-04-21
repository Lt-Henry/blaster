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

#ifndef BLASTER_TEXTURE
#define BLASTER_TEXTURE

#ifdef __cplusplus
extern "C" {
#endif

#include "color.h"

#include <stdint.h>

typedef struct {

    int width; /*! width in pixels */
    int height; /*! height in pixels */
    uint8_t type; /*! texture type (see constants.h) */
    uint8_t bpp; /*! Bytes per pixel (1,2,4) */
    
    void* data;

} bl_texture_t;

/*!
    Creates a new texture
*/
bl_texture_t* bl_texture_new(int width,int height, uint8_t type);

/*!
    Creates a new texture from with data copied inside
*/
bl_texture_t* bl_texture_new_from_data(int width,int height,uint8_t type,const void* data);

/*!
    Free a texture 
*/
void bl_texture_delete(bl_texture_t* t);

/*!
    Sets a 32 bit pixel
*/
void bl_texture_set_pixel(bl_texture_t* t,int x,int y,bl_pixel_t pixel);

/*!
    Gets a 32 bit pixel
*/
bl_pixel_t bl_texture_get_pixel(bl_texture_t* t,int x,int y);

#ifdef __cplusplus
}
#endif

#endif
