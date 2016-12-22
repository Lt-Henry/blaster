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

#include <blaster/raster.h>

#include <stdio.h>


static void bl_raster_clean_tiles()
{
	int N=raster->tiles_width*raster->tiles_height;
	
	for (int n=0;n<N;n++) {
		bl_tile_free(raster->tiles[n]);
	}

	free(raster->tiles);

}

bl_raster_t* bl_raster_new()
{
	bl_raster_t* raster;
	
	raster=malloc(sizeof(bl_raster_t));
	
	raster->tiles_width=0;
	raster->tiles_height=0;
	raster->tiles=NULL;
	
	return raster;
}

void bl_raster_delete(bl_raster_t* raster)
{
	bl_raster_clean_tiles();
	free(raster);
}

void bl_raster_resize(bl_raster_t* raster,int width,int height)
{
	bl_raster_clean_tiles();
	
	// compute number of tiles and real size (which may be smaller than requested)
	raster->tiles_width=width/BL_RASTER_TILE_SIZE;
	raster->tiles_height=height/BL_RASTER_TILE_SIZE;
	
	raster->screen_width=raster->tiles_width*BL_RASTER_TILE_SIZE;
	raster->screen_height=raster->tiles_height*BL_RASTER_TILE_SIZE;
	
	raster->tiles=malloc(sizeof(tile_t*)*raster->tiles_width*raster->tiles_height);
	
	for (int j=0;j<raster->tiles_height;j++) {
		for (int i=0;i<raster->tiles_width;i++) {
			int index = i+j*raster->tiles_width;
			
			int x=i*BL_RASTER_TILE_SIZE;
			int y=j*BL_RASTER_TILE_SIZE;

			raster->tiles[index]=bl_tile_new(x,y,BL_RASTER_TILE_SIZE,BL_RASTER_TILE_SIZE);
		}
	}
}

