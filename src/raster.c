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

bl_raster_t* bl_raster_new()
{
	bl_raster_t* raster;
	
	raster=malloc(sizeof(bl_raster_t));
	
	raster->tiles_width=0;
	raster->tiles_height=0;
	raster->tiles=NULL;
	
	return raster;
}

void bl_raster_free(bl_raster_t* raster)
{
	int N=raster->tiles_width*raster->tiles_height;

	free(raster);
}

void bl_raster_resize(bl_raster_t* raster,int width,int height)
{
}

