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

#include <blaster/matrix_stack.h>
#include <blaster/matrix.h>

#include <stdlib.h>

bl_matrix_stack_t* bl_matrix_stack_new(int size)
{
    bl_matrix_stack_t* stack=NULL;
    
    stack=malloc(sizeof(bl_matrix_stack_t));
    
    stack->size=size;
    stack->base=malloc(sizeof(float)*16*size);
    stack->index=0;
    stack->matrix=stack->base;
    
    return stack;
}

void bl_matrix_stack_delete(bl_matrix_stack_t* stack)
{
    free(stack->base);
    free(stack);
}

int bl_matrix_stack_push(bl_matrix_stack_t* stack)
{
    if (stack->index==(stack->size-1)) {
        return 0;
    }
    
    bl_matrix_copy(stack->matrix+16,stack->matrix);
    
    stack->index++;
    stack->matrix+=16;
    
    return 1;
}

int bl_matrix_stack_pop(bl_matrix_stack_t* stack)
{
    if (stack->index==0) {
        return 0;
    }
    
    stack->index--;
    stack->matrix-=16;
    
    return 1;
}

void bl_matrix_stack_load_identity(bl_matrix_stack_t* stack)
{
    bl_matrix_identity(stack->matrix);
}

void bl_matrix_stack_mult(bl_matrix_stack_t* stack,float* m)
{
    float a[16];

    bl_matrix_copy(a,stack->matrix);
    bl_matrix_mult(stack->matrix,a,m);
}

void bl_matrix_stack_translate(bl_matrix_stack_t* stack,float x,float y,float z)
{
    float a[16];
    float b[16];
    
    bl_matrix_copy(a,stack->matrix);
    bl_matrix_translate(b,x,y,z);
    
    bl_matrix_mult(stack->matrix,a,b);
}

void bl_matrix_stack_rotate_x(bl_matrix_stack_t* stack,float angle)
{
    float a[16];
    float b[16];
    
    bl_matrix_copy(a,stack->matrix);
    bl_matrix_rotate_x(b,angle);
    
    bl_matrix_mult(stack->matrix,a,b);
}

void bl_matrix_stack_rotate_y(bl_matrix_stack_t* stack,float angle)
{
    float a[16];
    float b[16];
    
    bl_matrix_copy(a,stack->matrix);
    bl_matrix_rotate_y(b,angle);
    
    bl_matrix_mult(stack->matrix,a,b);
}

void bl_matrix_stack_rotate_z(bl_matrix_stack_t* stack,float angle)
{
    float a[16];
    float b[16];
    
    bl_matrix_copy(a,stack->matrix);
    bl_matrix_rotate_z(b,angle);
    
    bl_matrix_mult(stack->matrix,a,b);
}

void bl_matrix_stack_scale(bl_matrix_stack_t* stack,float x,float y,float z)
{
    float a[16];
    float b[16];
    
    bl_matrix_copy(a,stack->matrix);
    bl_matrix_scale(b,x,y,z);
    
    bl_matrix_mult(stack->matrix,a,b);

}
