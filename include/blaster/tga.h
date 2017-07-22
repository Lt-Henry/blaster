/*
    blaster

    Copyright (C) 2017  Enrique Medina Gremaldos <quiqueiii@gmail.com>

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

#ifndef BLASTER_TGA
#define BLASTER_TGA

#ifdef __cplusplus
extern "C" {
#endif

#include "texture.h"

typedef struct __attribute__((__packed__)) {
    uint8_t id_size;
    uint8_t color_map_type;
    uint8_t image_type;
    
    uint16_t color_map_origin;
    uint16_t color_map_length;
    uint8_t color_map_entry;
    
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;

    uint8_t bpp;
    uint8_t descriptor;
    
} bl_tga_header_t;

/*!
    Create a new texture from a tga file
*/
bl_texture_t* bl_tga_load(const char* filename);

/*!
    Save a texture into a tga file
*/
void bl_tga_save(bl_texture_t* texture, const char* filename);


#ifdef __cplusplus
}
#endif

#endif
