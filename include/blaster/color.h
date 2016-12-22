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

#ifndef _BLASTER_COLOR_
#define _BLASTER_COLOR_

#include <stdint.h>

/*!
	Sets a float rgba color
*/
void bl_color_set(float* c,float r,float g,float b,float a);

/*!
	Adds two colors
*/
void bl_color_add(float* c,float* a,float* b);

/*!
	Substracts two colors
*/
void bl_color_sub(float* c,float* a,float* b);

/*!
	clamp color to [0,1] range
*/
void bl_color_clamp(float* c);

/*!
	color to pixel
*/
uint32_t bl_color_get_pixel(const float* c);

/*!
	pixel to color
*/
void bl_color_from_pixel(float* c,uint32_t pixel);

#endif
