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

#ifndef _BLASTER_TILE_
#define _BLASTER_TILE_

#include <stdint.h>

typedef struct {
	
	int width;
	int height;
	int x;
	int y;
	
	uint32_t* color;
	uint16_t* depth;
} bl_tile_t;

/*!
	Create a new tile
*/
bl_tile_t* bl_tile_new(int x,int y,int width,int height);

/*!
	Free tile
*/
void bl_tile_delete(bl_tile_t* tile);

/*!
	Clear both depth and color buffers
*/
void bl_tile_clear(bl_tile_t* tile,uint32_t color,uint16_t depth);

#endif

