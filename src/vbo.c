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

#include <stdarg.h>


bl_vbo_t* bl_vbo_new(size_t size,size_t vertex_size)
{
    bl_vbo_t* vbo=NULL;

    vbo=malloc(sizeof(bl_vbo_t));

    vbo->size=size;
    vbo->vertex_size=vertex_size;
    
    uint8_t* p = aligned_alloc(16,size*vertex_size);
    
    vbo->data=p;
    
    return vbo;
}

void bl_vbo_delete(bl_vbo_t* vbo)
{
    free(vbo->data);
    free(vbo);
}


void bl_vbo_set(bl_vbo_t* vbo,int attrib,int index,void* value)
{

    uint8_t* source = value;
    uint8_t* dest = vbo->data[attrib];
    
    size_t bytes = vbo->attrib_type[attrib]>>4;
    
    dest+=(bytes*index*vbo->attrib_size[attrib]);
    
    memcpy(dest,source,vbo->attrib_size[attrib]*bytes);
    
}
