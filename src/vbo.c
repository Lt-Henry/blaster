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
#include <string.h>


bl_vbo_t* bl_vbo_new(size_t size,size_t vertex_size)
{
    bl_vbo_t* vbo=NULL;

    vbo=malloc(sizeof(bl_vbo_t));

    vbo->size=size;
    vbo->vertex_size=vertex_size;
    
    uint8_t* p = malloc(size*vertex_size*sizeof(float));
    
    vbo->data=p;
    
    return vbo;
}

void bl_vbo_delete(bl_vbo_t* vbo)
{
    free(vbo->data);
    free(vbo);
}
/*
size_t bl_vbo_reconfigure(bl_vbo_t* vbo,size_t vertex_size)
{
    size_t new_size;
    
    new_size = vbo->size*vbo->vertex_size;
    new_size = new_size / vertex_size;
    
    vbo->size = new_size;
    vbo->vertex_size = vertex_size;
    
    return new_size;
}
*/
int bl_vbo_set(bl_vbo_t* vbo,int vertex,int index,float value)
{

    if (vertex>=vbo->size || index>=vbo->vertex_size) {
        return BL_INDEX_ERROR;
    }
    
    float* dest = (float*)vbo->data;
    
    dest+=vertex*vbo->vertex_size;
    dest[index]=value;
    
    return BL_OK;
}

int bl_vbo_set_v(bl_vbo_t* vbo,int vertex,void* value)
{
    if (vertex>=vbo->size) {
        return BL_INDEX_ERROR;
    }
    
    float* dest = (float*)vbo->data;
    
    dest+=vertex*vbo->vertex_size;
    memcpy(dest,value,vbo->vertex_size*sizeof(float));
    
    return BL_OK;
}

void* bl_vbo_get(bl_vbo_t* vbo,int vertex)
{
    if (vertex>=vbo->size) {
        return NULL;
    }
    
    return vbo->data + (vertex * vbo->vertex_size * sizeof(float));
}
