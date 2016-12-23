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

#ifndef _BLASTER_RASTER_
#define _BLASTER_RASTER_


#include "tile.h"

/*!
	Raster main structure
*/
typedef struct {

	int tiles_width;
	int tiles_height;
	int screen_width;
	int screen_height;
	
	bl_tile_t** tiles;
} bl_raster_t;

/*!
	Create a new raster
*/
bl_raster_t* bl_raster_new();

/*!
	Free raster
*/
void bl_raster_delete(bl_raster_t* raster);

/*!
	Resize framebuffer
*/
void bl_raster_resize(bl_raster_t* raster,int width,int height);

#endif

