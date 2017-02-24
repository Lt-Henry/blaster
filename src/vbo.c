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
#include <blaster/constants.h>


bl_vbo_t* bl_vbo_new(uint8_t primitive,size_t size,void* data)
{
	bl_vbo_t* vbo=NULL;
	
	vbo=malloc(sizeof(bl_vbo_t));
	
	memset(vbo,0,sizeof(bl_vbo_t));
	
	vbo->primitive=primitive;
	vbo->size=size;
	vbo->data=data;
	
	return vbo;
}

void bl_vbo_delete(bl_vbo_t* vbo)
{

	for (int n=0;n<8;n++) {
		if (vbo->attribute[n].name!=NULL) {
			free(vbo->attribute[n].name);
		}
	}
	
	free(vbo);
}

int bl_vbo_set_attribute(bl_vbo_t* vbo,int id,uint8_t type,uint8_t size,uint8_t settings,const char* name)
{
	if (id<0 || id>7) {
		return 1;
	}
	
	vbo->attribute[id].type=type;
	vbo->attribute[id].size=size;
	vbo->attribute[id].settings=settings;
	vbo->attribute[id].name=strdup(name);
	
	return 0;
}
