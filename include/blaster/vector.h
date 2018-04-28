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


#ifndef BLASTER_VECTOR
#define BLASTER_VECTOR

#ifdef __cplusplus
extern "C" {
#endif

#include "matrix.h"

typedef union bl_vector_u {

    float data[4];
    
    struct {
        float x;
        float y;
        float z;
        float w;
    };
} bl_vector_t;

/*!
    Sets vector values
*/
void bl_vector_set(bl_vector_t* v,float x,float y,float z,float w);

/*!
    Copy vectors
*/
void bl_vector_copy(bl_vector_t* d,bl_vector_t* s);

/*!
    Invert vector
*/
void bl_vector_invert(bl_vector_t* v);

/*!
    Adds vectors
*/
void bl_vector_add(bl_vector_t* r,bl_vector_t* a,bl_vector_t* b);

/*!
    Substracts vectors
*/
void bl_vector_sub(bl_vector_t* r,bl_vector_t* a,bl_vector_t* b);

/*!
    Dot product
*/
float bl_vector_dot(const bl_vector_t* a,const bl_vector_t* b);

/*!
    Cross product
*/
void bl_vector_cross(bl_vector_t* r,bl_vector_t* a,bl_vector_t* b);

/*!
    Vector Norm (euclidean distance)
*/
float bl_vector_norm(const bl_vector_t* v);

/*!
    Vector normalization
*/
void bl_vector_normalize(bl_vector_t* v);

/*!
    Divide by its W component
*/
void bl_vector_homogeneus(bl_vector_t* v);

/*!
    Vector Matrix product
*/
void bl_vector_mult(bl_vector_t* r,const bl_vector_t* v,const bl_matrix_t* m);

/*!
    Scale a vector
*/
void bl_vector_scale(bl_vector_t* v,float s);

#ifdef __cplusplus
}
#endif

#endif
