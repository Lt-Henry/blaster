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

/*!
    Sets vector values
*/
void bl_vector_set(float* v,float x,float y,float z,float w);

/*!
    Copy vectors
*/
void bl_vector_copy(float* d,float* s);

/*!
    Invert vector
*/
void bl_vector_invert(float* v);

/*!
    Adds vectors
*/
void bl_vector_add(float* r,float* a,float* b);

/*!
    Substracts vectors
*/
void bl_vector_sub(float* r,float* a,float* b);

/*!
    Dot product
*/
float bl_vector_dot(const float* a,const float* b);

/*!
    Cross product
*/
void bl_vector_cross(float* r,float* a,float* b);

/*!
    Vector Norm (euclidean distance)
*/
float bl_vector_norm(const float* v);

/*!
    Vector normalization
*/
void bl_vector_normalize(float* v);

/*!
    Divide by its W component
*/
void bl_vector_homogeneus(float* v);

/*!
    Vector Matrix product
*/
void bl_vector_mult(float* r,const float* v,const float* m);

/*!
    Scale a vector
*/
void bl_vector_scale(float* v,float s);

#ifdef __cplusplus
}
#endif

#endif
