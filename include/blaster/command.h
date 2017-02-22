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

#ifndef BLASTER_COMMAND
#define BLASTER_COMMAND

#ifdef __cplusplus
extern "C" {
#endif

#include "tile.h"
#include "texture.h"
#include "vbo.h"

#include <stdint.h>

/*! command structure */
typedef struct {

	/*! command type (see contants.h) */
	uint8_t type;
	
	union {
		
		struct {
			bl_tile_t* tile;
		} clear;
		
		struct {
			bl_tile_t* tile;
		} draw;
		
		struct {
			bl_texture_t* texture;
		} texture;
		
		struct {
			bl_vbo_t* vbo;
		} vbo;
		
		struct {
			uint32_t color;
		} clear_color;
	};
	
} bl_command_t;

#ifdef __cplusplus
}
#endif

#endif
