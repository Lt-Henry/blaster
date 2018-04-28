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

typedef struct bl_matrix_u {
    float data[16];
} bl_matrix_t;

/*!
    Sets a value in row/column
*/
void bl_matrix_set(bl_matrix_t* m,int r,int c,float v);

/*!
    Gets a value from row/column
*/
float bl_matrix_get(bl_matrix_t* m,int r,int c);

/*!
    Copy matrices
*/
void bl_matrix_copy(bl_matrix_t* d,bl_matrix_t* s);

/*!
    Sets identity matrix
*/
void bl_matrix_identity(bl_matrix_t* m);

/*!
    Sets translation matrix
*/
void bl_matrix_translate(bl_matrix_t* m,float x,float y,float z);

/*!
    Sets rotation matrix over X
*/
void bl_matrix_rotate_x(bl_matrix_t* m,float rads);

/*!
    Sets rotation matrix over Y
*/
void bl_matrix_rotate_y(bl_matrix_t* m,float rads);

/*!
    Sets rotation matrix over Z
*/
void bl_matrix_rotate_z(bl_matrix_t* m,float rads);

/*!
    Sets a scale matrix
*/
void bl_matrix_scale(bl_matrix_t* m,float x,float y,float z);

/*!
    Matrix multiplication
*/
void bl_matrix_mult(bl_matrix_t* m,bl_matrix_t* a,bl_matrix_t* b);

/*!
    Print matrix to stdout
*/
void bl_matrix_print(const bl_matrix_t* m);

#ifdef __cplusplus
}
#endif

#endif
