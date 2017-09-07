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

#ifndef BLASTER_MATRIX
#define BLASTER_MATRIX

#ifdef __cplusplus
extern "C" {
#endif


/*!
    Sets a value in row/column
*/
void bl_matrix_set(float* m,int r,int c,float v);

/*!
    Gets a value from row/column
*/
float bl_matrix_get(float* m,int r,int c);

/*!
    Copy matrices
*/
void bl_matrix_copy(float* d,float* s);

/*!
    Sets identity matrix
*/
void bl_matrix_identity(float* m);

/*!
    Sets translation matrix
*/
void bl_matrix_translate(float* m,float x,float y,float z);

/*!
    Sets rotation matrix over X
*/
void bl_matrix_rotate_x(float* m,float rads);

/*!
    Sets rotation matrix over Y
*/
void bl_matrix_rotate_y(float* m,float rads);

/*!
    Sets rotation matrix over Z
*/
void bl_matrix_rotate_z(float* m,float rads);

/*!
    Sets a scale matrix
*/
void bl_matrix_scale(float* m,float x,float y,float z);

/*!
    Matrix multiplication
*/
void bl_matrix_mult(float* m,float* a,float* b);

#ifdef __cplusplus
}
#endif

#endif
