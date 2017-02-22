/*
	blaster
	
	Copyright (C) 2016  Enrique Medina Gremaldos <quiqueiii@gmail.com>

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


#ifndef BLASTER_MATH
#define BLASTER_MATH

#ifdef __cplusplus
extern "C" {
#endif

/*!
	Sets vector values
*/
void bl_vec_set(float* v,float x,float y,float z,float w);

/*!
	Copy vectors
*/
void bl_vec_copy(float* d,float* s);

/*!
	Adds vectors
*/
void bl_vec_add(float* r,float* a,float* b);

/*!
	Substracts vectors
*/
void bl_vec_sub(float* r,float* a,float* b);

/*!
	Dot product
*/
float bl_vec_dot(const float* a,const float* b);

/*!
	Cross product
*/
void bl_vec_cross(float* r,float* a,float* b);

/*!
	Vector Norm (euclidean distance)
*/
float bl_vec_norm(const float* v);

/*!
	Vector normalization
*/
void bl_vec_normalize(float* v);

/*!
	Divide by its W component
*/
void bl_vec_homogeneus(float* v);

/*!
	Vector Matrix product
*/
void bl_vec_mult(float* r,const float* v,const float* m);

/*!
	Copy matrices
*/
void bl_mat_copy(float* d,float* s);

/*!
	Sets identity matrix
*/
void bl_mat_identity(float* m);

/*!
	Sets translation matrix
*/
void bl_mat_translation(float* m,float x,float y,float z);

/*!
	Sets rotation matrix over X
*/
void bl_mat_rotation_x(float* m,float rads);

/*!
	Sets rotation matrix over Y
*/
void bl_mat_rotation_y(float* m,float rads);

/*!
	Sets rotation matrix over Z
*/
void bl_mat_rotation_z(float* m,float rads);

/*!
	Matrix multiplication
*/
void bl_mat_mult(float* m,float* a,float* b);

#ifdef __cplusplus
}
#endif

#endif
