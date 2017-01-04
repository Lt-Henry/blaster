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


#include <blaster/vbo.h>


bl_vbo_t* bl_vbo_new(uint32_t type,size_t size, uint32_t properties)
{
	bl_vbo_t* vbo;
	
	vbo=malloc(sizeof(bl_vbo_t));
	
	vbo->type=type;
	vbo->size=size;
	vbo->properties=properties;
	vbo->vertices=NULL;
	vbo->normals=NULL;
	vbo->colors=NULL;
	vbo->uvs=NULL;
	
	size_t real_size=0;
	
	switch (type) {
		case BL_VBO_POINTS:
			real_size=size;
		break;
		
		case BL_VBO_LINES:
			real_size=size*2;
		break;
		
		case BL_VBO_TRIANGLES:
			real_size=size*3;
		break;
		
		default:
			real_size=size;
	}
	
	if (properties & BL_VBO_VERTEX) {
		// x y z w
		vbo->vertices=malloc(sizeof(float)*real_size*4);
	}
	
	if (properties & BL_VBO_NORMAL) {
		// x y z w
		vbo->normals=malloc(sizeof(float)*real_size*4);
	}
	
	if (properties & BL_VBO_COLOR) {
		// r g b a
		vbo->colors=malloc(sizeof(float)*real_size*4);
	}
	
	if (properties & BL_VBO_UV) {
		// u v
		vbo->uvs=malloc(sizeof(float)*real_size*2);
	}
	
	return vbo;
}

void bl_vbo_delete(bl_vbo_t* vbo)
{
	if (vbo->vertices!=NULL) {
		free(vbo->vertices);
	}
	
	if (vbo->normals!=NULL) {
		free(vbo->normals);
	}
	
	if (vbo->colors!=NULL) {
		free(vbo->colors);
	}
	
	if (vbo->uvs!=NULL) {
		free(vbo->uvs);
	}

	free(vbo);

}

float* bl_vbo_get(bl_vbo_t* vbo,uint32_t property,int index)
{
	size_t factor=0;
	
	switch (vbo->type) {
		case BL_VBO_POINTS:
			factor=1;
		break;
		
		case BL_VBO_LINES:
			factor=2;
		break;
		
		case BL_VBO_TRIANGLES:
			factor=3;
		break;
		
		default:
			factor=1;
	}
	
	float* base=NULL;
	
	switch (property) {
	
		case BL_VBO_VERTEX:
			base=vbo->vertices;
			factor*=4;
		break;
		
		case BL_VBO_NORMAL:
			base=vbo->normals;
			factor*=4;
		break;

		case BL_VBO_COLOR:
			base=vbo->colors;
			factor*=4;
		break;
		
		case BL_VBO_UV:
			base=vbo->uvs;
			factor*=2;
		break;

		default:
			base=vbo->vertices;
			factor*=4;

	}
	
	base+=(factor*index);
	
	return base;
}

