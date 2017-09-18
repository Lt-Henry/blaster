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


bl_vbo_t* bl_vbo_new(size_t size,size_t attributes)
{
    bl_vbo_t* vbo=NULL;

    vbo=malloc(sizeof(bl_vbo_t));

    vbo->capacity=size;
    vbo->size=0;
    vbo->attributes=attributes;
    vbo->data=malloc(sizeof(float)*size*attributes);

    return vbo;
}

void bl_vbo_delete(bl_vbo_t* vbo)
{
    free(vbo->data);
    free(vbo);
}

void bl_vbo_clear(bl_vbo_t* vbo)
{
    vbo->size=0;
}

void bl_vbo_add(bl_vbo_t* vbo,...)
{
    float* ptr = &vbo->data[vbo->size*vbo->attributes];
    
    va_list args;
    
    va_start(args,vbo);
    
    for (int n=0;n<vbo->attributes;n++) {
        ptr[n]=va_arg(args,float);
    }
    
    va_end(args);
    
    vbo->size++;
}
