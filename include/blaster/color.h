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

#ifndef BLASTER_COLOR
#define BLASTER_COLOR

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union bl_color_u {
    
    float channel[4];
    
    struct {
        float r;
        float g;
        float b;
        float a;
    };
    
} bl_color_t;

typedef union bl_pixel_u {
    uint32_t value;
    uint8_t channel[4];
} bl_pixel_t;

/*!
    Sets a float rgba color
*/
void bl_color_set(bl_color_t* c,float r,float g,float b,float a);

/*!
    Adds two colors
*/
void bl_color_add(bl_color_t* c,bl_color_t* a,bl_color_t* b);

/*!
    Substracts two colors
*/
void bl_color_sub(bl_color_t* c,bl_color_t* a,bl_color_t* b);

/*!
    Mult a color by a scalar
*/
void bl_color_scale(bl_color_t* c,float s);

/*!
    Mult two colors
*/
void bl_color_mult(bl_color_t* c,bl_color_t* a,bl_color_t* b);

/*!
    clamp color to [0,1] range
*/
void bl_color_clamp(bl_color_t* c);

/*!
    color to pixel
*/
bl_pixel_t bl_color_get_pixel(const bl_color_t* c);

/*!
    pixel to color
*/
void bl_color_from_pixel(bl_color_t* c,bl_pixel_t pixel);

/*!
    create color from bytes
*/
void bl_color_from_ub(bl_color_t* c,uint8_t r,uint8_t g,uint8_t b,uint8_t a);

#ifdef __cplusplus
}
#endif

#endif
