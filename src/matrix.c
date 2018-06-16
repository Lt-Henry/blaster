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

#include <blaster/optimization.h>
#include <blaster/matrix.h>

#include <math.h>
#include <stdio.h>


void bl_matrix_set(bl_matrix_t* m,int r,int c,float v)
{
    m->data[c+(r*4)]=v;
}

float bl_matrix_get(bl_matrix_t* m,int r,int c)
{
    return m->data[c+(r*4)];
}

void bl_matrix_copy(bl_matrix_t* d,bl_matrix_t* s)
{

    for (int n=0;n<16;n++) {
        d->data[n]=s->data[n];
    }
}

void bl_matrix_identity(bl_matrix_t* m)
{
    m->data[0]=1.0f;
    m->data[1]=0.0f;
    m->data[2]=0.0f;
    m->data[3]=0.0f;

    m->data[4]=0.0f;
    m->data[5]=1.0f;
    m->data[6]=0.0f;
    m->data[7]=0.0f;

    m->data[8]=0.0f;
    m->data[9]=0.0f;
    m->data[10]=1.0f;
    m->data[11]=0.0f;

    m->data[12]=0.0f;
    m->data[13]=0.0f;
    m->data[14]=0.0f;
    m->data[15]=1.0f;
}

void bl_matrix_translate(bl_matrix_t* m,float x,float y,float z)
{
    m->data[0]=1.0f;
    m->data[1]=0.0f;
    m->data[2]=0.0f;
    m->data[3]=0.0f;

    m->data[4]=0.0f;
    m->data[5]=1.0f;
    m->data[6]=0.0f;
    m->data[7]=0.0f;

    m->data[8]=0.0f;
    m->data[9]=0.0f;
    m->data[10]=1.0f;
    m->data[11]=0.0f;

    m->data[12]=x;
    m->data[13]=y;
    m->data[14]=z;
    m->data[15]=1.0f;
}

void bl_matrix_rotate_x(bl_matrix_t* m,float rads)
{
    m->data[0]=1.0f;
    m->data[1]=0.0f;
    m->data[2]=0.0f;
    m->data[3]=0.0f;

    m->data[4]=0.0f;
    m->data[5]=cosf(rads);
    m->data[6]=sinf(rads);
    m->data[7]=0.0f;

    m->data[8]=0.0f;
    m->data[9]=-sinf(rads);
    m->data[10]=cosf(rads);
    m->data[11]=0.0f;

    m->data[12]=0.0f;
    m->data[13]=0.0f;
    m->data[14]=0.0f;
    m->data[15]=1.0f;
}

void bl_matrix_rotate_y(bl_matrix_t* m,float rads)
{
    m->data[0]=cosf(rads);
    m->data[1]=0.0f;
    m->data[2]-=sinf(rads);
    m->data[3]=0.0f;

    m->data[4]=0.0f;
    m->data[5]=1.0f;
    m->data[6]=0.0f;
    m->data[7]=0.0f;

    m->data[8]=sinf(rads);
    m->data[9]=0.0f;
    m->data[10]=cosf(rads);
    m->data[11]=0.0f;

    m->data[12]=0.0f;
    m->data[13]=0.0f;
    m->data[14]=0.0f;
    m->data[15]=1.0f;
}

void bl_matrix_rotate_z(bl_matrix_t* m,float rads)
{
    m->data[0]=cosf(rads);
    m->data[1]=sinf(rads);
    m->data[2]=0.0f;
    m->data[3]=0.0f;

    m->data[4]=-sinf(rads);
    m->data[5]=cosf(rads);
    m->data[6]=0.0f;
    m->data[7]=0.0f;

    m->data[8]=0.0f;
    m->data[9]=0.0f;
    m->data[10]=1.0f;
    m->data[11]=0.0f;

    m->data[12]=0.0f;
    m->data[13]=0.0f;
    m->data[14]=0.0f;
    m->data[15]=1.0f;
}

void bl_matrix_scale(bl_matrix_t* m,float x,float y,float z)
{
    m->data[0]=x;
    m->data[1]=0.0f;
    m->data[2]=0.0f;
    m->data[3]=0.0f;

    m->data[4]=0.0f;
    m->data[5]=y;
    m->data[6]=0.0f;
    m->data[7]=0.0f;

    m->data[8]=0.0f;
    m->data[9]=0.0f;
    m->data[10]=z;
    m->data[11]=0.0f;

    m->data[12]=0.0f;
    m->data[13]=0.0f;
    m->data[14]=0.0f;
    m->data[15]=1.0f;

}

#ifdef BLASTER_MATRIX_MULT_FMA
void bl_matrix_mult(bl_matrix_t* m,bl_matrix_t* a,bl_matrix_t* b)
{
    __m128 A;
    __m128 B;
    __m128 R;
    __m128 Q;
    
    for (size_t i=0;i<16;i+=4) {
        A=_mm_loadu_ps(a->data);
        B=_mm_load_ps1(b->data+i);
            
        R=_mm_mul_ps(A,B);
        
        for (size_t j=1;j<4;j++) {
            size_t k=j<<2;
            A=_mm_loadu_ps(a->data+k);
            B=_mm_load_ps1(b->data+i+j);
            
            R=_mm_fmadd_ps(A,B,R);
        }
        
        _mm_store_ps(m->data+i,R);
    }
}
#endif

#ifdef BLASTER_MATRIX_MULT_SSE
void bl_matrix_mult(bl_matrix_t* m,bl_matrix_t* a,bl_matrix_t* b)
{
    __m128 A;
    __m128 B;
    __m128 R;
    __m128 Q;
    
    for (size_t i=0;i<16;i+=4) {
        A=_mm_loadu_ps(a->data);
        B=_mm_load_ps1(b->data+i);
            
        R=_mm_mul_ps(A,B);
        
        for (size_t j=1;j<4;j++) {
            size_t k=j<<2;
            A=_mm_loadu_ps(a->data+k);
            B=_mm_load_ps1(b->data+i+j);
            
            Q=_mm_mul_ps(A,B);
            R=_mm_add_ps(R,Q);
        }
        
        _mm_store_ps(m->data+i,R);
    }
}
#endif

#ifdef BLASTER_MATRIX_MULT_GENERIC
void bl_matrix_mult(bl_matrix_t* m,bl_matrix_t* a,bl_matrix_t* b)
{
    for (int i=0;i<16;i+=4) {
        for (int j=0;j<4;j++) {
            m->data[i + j] = (b->data[i + 0] * a->data[j + 0])
            + (b->data[i + 1] * a->data[j + 4])
            + (b->data[i + 2] * a->data[j + 8])
            + (b->data[i + 3] * a->data[j + 12]);
        }
    }
}
#endif

void bl_matrix_print(const bl_matrix_t* m)
{
    for (int n=0;n<4;n++) {
    
        printf("[%.2f %.2f %.2f %.2f]\n",m->data[n],m->data[n+4],m->data[n+8],m->data[n+12]);
    }
}
