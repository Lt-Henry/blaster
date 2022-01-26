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

    /*! number of vertices */
    size_t size;

    /*! number of elements per vertex (coord,color,normal,...) */
    size_t vertex_size;
    
    /*! pointer to vbo data */
    void* data; 

} bl_vbo_t;

/*!
    Create a new vbo of given size
*/
bl_vbo_t* bl_vbo_new(size_t size,size_t vertex_size);

/*!
    Destroys vbo
*/
void bl_vbo_delete(bl_vbo_t* vbo);

/*!
    Reconfigure vbo for a new vertex size. No reallocation is done.
    \return The new number of vertices vbo can hold
*/
//size_t bl_vbo_reconfigure(bl_vbo_t* vbo,size_t vertex_size);

/*!
    Sets a vertex with all it's attributes
*/
int bl_vbo_set(bl_vbo_t* vbo,int vertex,int index,float value);

int bl_vbo_set_v(bl_vbo_t* vbo,int vertex,void* value);

#ifdef __cplusplus
}
#endif

#endif
