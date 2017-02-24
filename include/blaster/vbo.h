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

#ifndef BLASTER_VBO
#define BLASTER_VBO

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

typedef struct {
	/*! number of values */
	uint8_t size;
	
	/*! data type */
	uint8_t type;
	
	/*! settings mask */
	uint8_t settings;
	
	/*! attribute name */
	char* name;
} bl_attribute_t;

typedef struct {
	/*! primitive type (triangles,lines,points) */
	uint8_t primitive;
	
	/*! number of primitives */
	size_t size;
	
	/*! struct storing attribute settings */
	bl_attribute_t attribute[8];
	
	/*! pointer to vbo data */
	void* data; 
	
	
} bl_vbo_t;

/*!
	Create a new vbo of given size
*/
bl_vbo_t* bl_vbo_new(uint8_t primitive,size_t size,void* data);

/*!
	Destroys vbo
*/
void bl_vbo_delete(bl_vbo_t* vbo);


/*!
	Sets up and attribute
	\param id an integer from 0 to 7
	\param type the type of the attribute, usually BL_FLOAT
	\param size number of values, ie: 2 for a UV coords
	\param name just a representative name
*/
int bl_vbo_set_attribute(bl_vbo_t* vbo,int id,uint8_t type,uint8_t size,uint8_t settings,const char* name);

#ifdef __cplusplus
}
#endif

#endif
