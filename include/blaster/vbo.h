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

    /*! real number of vertices */
    size_t size;

    uint8_t attrib_size[8];
    uint8_t attrib_type[8];
    
    /*! pointer to vbo data */
    void* data[8]; 

} bl_vbo_t;

/*!
    Create a new vbo of given size
*/
bl_vbo_t* bl_vbo_new(size_t size,uint8_t attributes[16]);

/*!
    Destroys vbo
*/
void bl_vbo_delete(bl_vbo_t* vbo);


void bl_vbo_set(bl_vbo_t* vbo,int attrib,int index,void* value);

#ifdef __cplusplus
}
#endif

#endif
