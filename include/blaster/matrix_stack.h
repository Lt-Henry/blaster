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


#ifndef BLASTER_MATRIX_STACK
#define BLASTER_MATRIX_STACK

#ifdef __cplusplus
extern "C" {
#endif

#include "matrix.h"

typedef struct {
    int size;
    int index;
    bl_matrix_t* base;
    bl_matrix_t* matrix;
} bl_matrix_stack_t;

/*!
    Creates a new matrix stack
*/
bl_matrix_stack_t* bl_matrix_stack_new(int size);

/*!
    Destroys the matrix stack
*/
void bl_matrix_stack_delete(bl_matrix_stack_t* stack);

/*!
    Adds a new matrix on top, copied from below
*/
int bl_matrix_stack_push(bl_matrix_stack_t* stack);

/*!
    Pops top matrix
*/
int bl_matrix_stack_pop(bl_matrix_stack_t* stack);

/*!
    Loads identity matrix on top
*/
void bl_matrix_stack_load_identity(bl_matrix_stack_t* stack);

/*!
    Mult top matrix
*/
void bl_matrix_stack_mult(bl_matrix_stack_t* stack,bl_matrix_t* m);

/*!
    Performs a translation matrix multiplication
*/
void bl_matrix_stack_translate(bl_matrix_stack_t* stack,float x,float y,float z);

/*!
    performs a rotation multiplication arround x axis
*/
void bl_matrix_stack_rotate_x(bl_matrix_stack_t* stack,float angle);

/*!
    performs a rotation multiplication arround y axis
*/
void bl_matrix_stack_rotate_y(bl_matrix_stack_t* stack,float angle);

/*!
    performs a rotation multiplication arround z axis
*/
void bl_matrix_stack_rotate_z(bl_matrix_stack_t* stack,float angle);

/*!
    Performs a scale matrix multiplication
*/
void bl_matrix_stack_scale(bl_matrix_stack_t* stack,float x,float y,float z);

/*!
    Performs a Frustum matrix multiplication
*/
void bl_matrix_stack_frustum(bl_matrix_stack_t* stack,
                            float left,float right,
                            float top, float bottom,
                            float near,float far
                            );

#ifdef __cplusplus
}
#endif

#endif
