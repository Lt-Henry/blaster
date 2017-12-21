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

#include <blaster/matrix.h>

#include <math.h>
#include <stdio.h>

#ifdef OPT_SSE

#include <xmmintrin.h>
#include <pmmintrin.h>

#endif

void bl_matrix_set(float* m,int r,int c,float v)
{
    m[c+(r*4)]=v;
}

float bl_matrix_get(float* m,int r,int c)
{
    return m[c+(r*4)];
}

void bl_matrix_copy(float* d,float* s)
{

    for (int n=0;n<16;n++) {
        d[n]=s[n];
    }
}

void bl_matrix_identity(float* m)
{
    m[0]=1.0f;
    m[1]=0.0f;
    m[2]=0.0f;
    m[3]=0.0f;

    m[4]=0.0f;
    m[5]=1.0f;
    m[6]=0.0f;
    m[7]=0.0f;

    m[8]=0.0f;
    m[9]=0.0f;
    m[10]=1.0f;
    m[11]=0.0f;

    m[12]=0.0f;
    m[13]=0.0f;
    m[14]=0.0f;
    m[15]=1.0f;
}

void bl_matrix_translate(float* m,float x,float y,float z)
{
    m[0]=1.0f;
    m[1]=0.0f;
    m[2]=0.0f;
    m[3]=0.0f;

    m[4]=0.0f;
    m[5]=1.0f;
    m[6]=0.0f;
    m[7]=0.0f;

    m[8]=0.0f;
    m[9]=0.0f;
    m[10]=1.0f;
    m[11]=0.0f;

    m[12]=x;
    m[13]=y;
    m[14]=z;
    m[15]=1.0f;
}

void bl_matrix_rotate_x(float* m,float rads)
{
    m[0]=1.0f;
    m[1]=0.0f;
    m[2]=0.0f;
    m[3]=0.0f;

    m[4]=0.0f;
    m[5]=cosf(rads);
    m[6]=sinf(rads);
    m[7]=0.0f;

    m[8]=0.0f;
    m[9]=-sinf(rads);
    m[10]=cosf(rads);
    m[11]=0.0f;

    m[12]=0.0f;
    m[13]=0.0f;
    m[14]=0.0f;
    m[15]=1.0f;
}

void bl_matrix_rotate_y(float* m,float rads)
{
    m[0]=cosf(rads);
    m[1]=0.0f;
    m[2]-=sinf(rads);
    m[3]=0.0f;

    m[4]=0.0f;
    m[5]=1.0f;
    m[6]=0.0f;
    m[7]=0.0f;

    m[8]=sinf(rads);
    m[9]=0.0f;
    m[10]=cosf(rads);
    m[11]=0.0f;

    m[12]=0.0f;
    m[13]=0.0f;
    m[14]=0.0f;
    m[15]=1.0f;
}

void bl_matrix_rotate_z(float* m,float rads)
{
    m[0]=cosf(rads);
    m[1]=sinf(rads);
    m[2]=0.0f;
    m[3]=0.0f;

    m[4]=-sinf(rads);
    m[5]=cosf(rads);
    m[6]=0.0f;
    m[7]=0.0f;

    m[8]=0.0f;
    m[9]=0.0f;
    m[10]=1.0f;
    m[11]=0.0f;

    m[12]=0.0f;
    m[13]=0.0f;
    m[14]=0.0f;
    m[15]=1.0f;
}

void bl_matrix_scale(float* m,float x,float y,float z)
{
    m[0]=x;
    m[1]=0.0f;
    m[2]=0.0f;
    m[3]=0.0f;

    m[4]=0.0f;
    m[5]=y;
    m[6]=0.0f;
    m[7]=0.0f;

    m[8]=0.0f;
    m[9]=0.0f;
    m[10]=z;
    m[11]=0.0f;

    m[12]=0.0f;
    m[13]=0.0f;
    m[14]=0.0f;
    m[15]=1.0f;

}

void bl_matrix_mult(float* m,float* a,float* b)
{

#ifdef OPT_SSE

    __m128 A;
    __m128 B;
    __m128 R;
    __m128 Q;
    
    for (size_t i=0;i<16;i+=4) {
        A=_mm_loadu_ps(a);
        B=_mm_load_ps1(b+i);
            
        R=_mm_mul_ps(A,B);
        
        for (size_t j=1;j<4;j++) {
            size_t k=j<<2;
            A=_mm_loadu_ps(a+k);
            B=_mm_load_ps1(b+i+j);
            
            Q=_mm_mul_ps(A,B);
            R=_mm_add_ps(R,Q);
        }
        
        _mm_store_ps(m+i,R);
    }

#else

    for (int i=0;i<16;i+=4) {
        for (int j=0;j<4;j++) {
            m[i + j] = (b[i + 0] * a[j + 0])
            + (b[i + 1] * a[j + 4])
            + (b[i + 2] * a[j + 8])
            + (b[i + 3] * a[j + 12]);
        }
    }
    
#endif

}

void bl_matrix_print(const float* m)
{
    for (int n=0;n<4;n++) {
    
        printf("[%.2f %.2f %.2f %.2f]\n",m[n],m[n+4],m[n+8],m[n+12]);
    }
}
