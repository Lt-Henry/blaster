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


#include <blaster/texture.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bl_texture_t* bl_texture_new(int width,int height)
{
	bl_texture_t* texture = NULL;
	
	texture = malloc(sizeof(bl_texture_t));
	
	texture->data=malloc(sizeof(uint32_t)*width*height);
	texture->width=width;
	texture->height=height;
	
	return texture;
}

bl_texture_t* bl_texture_new_from_data(int width,int height,const void* data)
{
	bl_texture_t* texture = bl_texture_new(width,height);
	memcpy((void*)texture->data,data,sizeof(uint32_t)*width*height);
	
	return texture;
}

void bl_texture_free(bl_texture_t* t)
{
	free(t->data);
	free(t);
}

void bl_texture_set_pixel(bl_texture_t* t,int x,int y,uint32_t pixel)
{
	t->data[x+y*t->width]=pixel;
}

uint32_t bl_texture_get_pixel(bl_texture_t* t,int x,int y)
{
	return t->data[x+y*t->width];
}
