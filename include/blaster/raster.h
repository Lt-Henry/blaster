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

#include "tile.h"
#include "command.h"

/*!
	Raster main structure
*/
typedef struct {

	/*! number of tiles in x */
	int tiles_width;
	
	/*! number of tiles in y */
	int tiles_height;
	
	/*! screen width in pixels */
	int screen_width;
	
	/*! screen height in pixels */
	int screen_height;
	
	bl_tile_t** tiles;
	
	/*! Command ring buffer */
	bl_command_buffer_t* cmd_queue;
	
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

/*!
	Sets clear pixel color
*/
void bl_raster_set_clear_color(bl_raster_t* raster,uint32_t color);

/*!
	Clear buffers
*/
void bl_raster_clear(bl_raster_t* raster);

/*!
	Update (flush all commands and render result)
*/
void bl_raster_update(bl_raster_t* raster);

/*!
	Get width in pixels, which should be a multiple of tiles_width
*/
int bl_raster_get_width(bl_raster_t* raster);

/*!
	Get height in pixels, which should be a multiple of tiles_height
*/
int bl_raster_get_height(bl_raster_t* raster);

#ifdef __cplusplus
}
#endif

#endif

