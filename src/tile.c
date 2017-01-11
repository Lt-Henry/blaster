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


#include <blaster/tile.h>

#include <stdlib.h>


bl_tile_t* bl_tile_new(int x,int y,int width,int height)
{
	bl_tile_t* tile;
	
	tile=malloc(sizeof(bl_tile_t));
	
	tile->x=x;
	tile->y=y;
	tile->width=width;
	tile->height=height;
	
	tile->color=malloc(sizeof(uint32_t)*width*height);
	tile->depth=malloc(sizeof(uint16_t)*width*height);
	
	return tile;
}

void bl_tile_delete(bl_tile_t* tile)
{
	free(tile->color);
	free(tile->depth);
	free(tile);
}

void bl_tile_clear(bl_tile_t* tile,uint32_t color,uint16_t depth)
{
	for (int j=0;j<tile->height;j++) {
		for (int i=0;i<tile->width;i++) {
			int index=i+j*tile->width;
			tile->color[index]=color;
			tile->depth[index]=depth;
		}
	}
}
