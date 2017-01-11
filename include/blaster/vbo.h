/*
	blaster
	
	Copyright (C) 2017  Enrique Medina Gremaldos <quiqueiii@gmail.com>

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

#ifndef _BLASTER_VBO_
#define _BLASTER_VBO_

#include <stdint.h>
#include <stdlib.h>

typedef struct {
	uint32_t type;
	uint32_t properties;
	size_t size;
	float* vertices;
	float* normals;
	float* colors;
	float* uvs;
	
} bl_vbo_t;

/*!
	Create a new vbo of given size
*/
bl_vbo_t* bl_vbo_new(uint32_t type,size_t size, uint32_t properties);

/*!
	Destroys vbo
*/
void bl_vbo_delete(bl_vbo_t* vbo);

/*!
	Gets pointer to Nth element
*/
float* bl_vbo_get(bl_vbo_t* vbo,uint32_t property,int index);

#endif
