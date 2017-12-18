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
#include <xmmintrin.h>
#include <pmmintrin.h>

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
    m[5]=cos(rads);
    m[6]=sin(rads);
    m[7]=0.0f;

    m[8]=0.0f;
    m[9]=-sin(rads);
    m[10]=cos(rads);
    m[11]=0.0f;

    m[12]=0.0f;
    m[13]=0.0f;
    m[14]=0.0f;
    m[15]=1.0f;
}

void bl_matrix_rotate_y(float* m,float rads)
{
    m[0]=cos(rads);
    m[1]=0.0f;
    m[2]-=sin(rads);
    m[3]=0.0f;

    m[4]=0.0f;
    m[5]=1.0f;
    m[6]=0.0f;
    m[7]=0.0f;

    m[8]=sin(rads);
    m[9]=0.0f;
    m[10]=cos(rads);
    m[11]=0.0f;

    m[12]=0.0f;
    m[13]=0.0f;
    m[14]=0.0f;
    m[15]=1.0f;
}

void bl_matrix_rotate_z(float* m,float rads)
{
    m[0]=cos(rads);
    m[1]=sin(rads);
    m[2]=0.0f;
    m[3]=0.0f;

    m[4]=-sin(rads);
    m[5]=cos(rads);
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
/*
    for (int i=0;i<16;i+=4) {
        for (int j=0;j<4;j++) {
            m[i + j] = (b[i + 0] * a[j + 0])
            + (b[i + 1] * a[j + 4])
            + (b[i + 2] * a[j + 8])
            + (b[i + 3] * a[j + 12]);
        }
    }
*/
/*
    m[0] = (b[0 + 0] * a[0 + 0]) + (b[0 + 1] * a[0 + 4]) + (b[0 + 2] * a[0 + 8]) + (b[0 + 3] * a[0 + 12]);
    m[1] = (b[0 + 0] * a[1 + 0]) + (b[0 + 1] * a[1 + 4]) + (b[0 + 2] * a[1 + 8]) + (b[0 + 3] * a[1 + 12]);
    m[2] = (b[0 + 0] * a[2 + 0]) + (b[0 + 1] * a[2 + 4]) + (b[0 + 2] * a[2 + 8]) + (b[0 + 3] * a[2 + 12]);
    m[3] = (b[0 + 0] * a[3 + 0]) + (b[0 + 1] * a[3 + 4]) + (b[0 + 2] * a[3 + 8]) + (b[0 + 3] * a[3 + 12]);
    
    m[4] = (b[4 + 0] * a[0 + 0]) + (b[4 + 1] * a[0 + 4]) + (b[4 + 2] * a[0 + 8]) + (b[4 + 3] * a[0 + 12]);
    m[5] = (b[4 + 0] * a[1 + 0]) + (b[4 + 1] * a[1 + 4]) + (b[4 + 2] * a[1 + 8]) + (b[4 + 3] * a[1 + 12]);
    m[6] = (b[4 + 0] * a[2 + 0]) + (b[4 + 1] * a[2 + 4]) + (b[4 + 2] * a[2 + 8]) + (b[4 + 3] * a[2 + 12]);
    m[7] = (b[4 + 0] * a[3 + 0]) + (b[4 + 1] * a[3 + 4]) + (b[4 + 2] * a[3 + 8]) + (b[4 + 3] * a[3 + 12]);
    
    m[8] = (b[8 + 0] * a[0 + 0]) + (b[8 + 1] * a[0 + 4]) + (b[8 + 2] * a[0 + 8]) + (b[8 + 3] * a[0 + 12]);
    m[9] = (b[8 + 0] * a[1 + 0]) + (b[8 + 1] * a[1 + 4]) + (b[8 + 2] * a[1 + 8]) + (b[8 + 3] * a[1 + 12]);
    m[10] = (b[8 + 0] * a[2 + 0]) + (b[8 + 1] * a[2 + 4]) + (b[8 + 2] * a[2 + 8]) + (b[8 + 3] * a[2 + 12]);
    m[11] = (b[8 + 0] * a[3 + 0]) + (b[8 + 1] * a[3 + 4]) + (b[8 + 2] * a[3 + 8]) + (b[8 + 3] * a[3 + 12]);
    
    m[12] = (b[12 + 0] * a[0 + 0]) + (b[12 + 1] * a[0 + 4]) + (b[12 + 2] * a[0 + 8]) + (b[12 + 3] * a[0 + 12]);
    m[13] = (b[12 + 0] * a[1 + 0]) + (b[12 + 1] * a[1 + 4]) + (b[12 + 2] * a[1 + 8]) + (b[12 + 3] * a[1 + 12]);
    m[14] = (b[12 + 0] * a[2 + 0]) + (b[12 + 1] * a[2 + 4]) + (b[12 + 2] * a[2 + 8]) + (b[12 + 3] * a[2 + 12]);
    m[15] = (b[12 + 0] * a[3 + 0]) + (b[12 + 1] * a[3 + 4]) + (b[12 + 2] * a[3 + 8]) + (b[12 + 3] * a[3 + 12]);
*/
/*
    float* a0=a+0;
    float* a1=a+4;
    float* a2=a+8;
    float* a3=a+12;
    
    float* bb=b+0;
    
    m[0]=(bb[0]*a0[0]) + (bb[1]*a1[0]) + (bb[2]*a2[0]) + (bb[3]*a3[0]);
    m[1]=(bb[0]*a0[1]) + (bb[1]*a1[1]) + (bb[2]*a2[1]) + (bb[3]*a3[1]);
    m[2]=(bb[0]*a0[2]) + (bb[1]*a1[2]) + (bb[2]*a2[2]) + (bb[3]*a3[2]);
    m[3]=(bb[0]*a0[3]) + (bb[1]*a1[3]) + (bb[2]*a2[3]) + (bb[3]*a3[3]);
    
    bb+=4;
    
    m[4]=(bb[0]*a0[0]) + (bb[1]*a1[0]) + (bb[2]*a2[0]) + (bb[3]*a3[0]);
    m[5]=(bb[0]*a0[1]) + (bb[1]*a1[1]) + (bb[2]*a2[1]) + (bb[3]*a3[1]);
    m[6]=(bb[0]*a0[2]) + (bb[1]*a1[2]) + (bb[2]*a2[2]) + (bb[3]*a3[2]);
    m[7]=(bb[0]*a0[3]) + (bb[1]*a1[3]) + (bb[2]*a2[3]) + (bb[3]*a3[3]);
    
    bb+=4;
    
    m[8]=(bb[0]*a0[0]) + (bb[1]*a1[0]) + (bb[2]*a2[0]) + (bb[3]*a3[0]);
    m[9]=(bb[0]*a0[1]) + (bb[1]*a1[1]) + (bb[2]*a2[1]) + (bb[3]*a3[1]);
    m[10]=(bb[0]*a0[2]) + (bb[1]*a1[2]) + (bb[2]*a2[2]) + (bb[3]*a3[2]);
    m[11]=(bb[0]*a0[3]) + (bb[1]*a1[3]) + (bb[2]*a2[3]) + (bb[3]*a3[3]);
    
    bb+=4;
    
    m[12]=(bb[0]*a0[0]) + (bb[1]*a1[0]) + (bb[2]*a2[0]) + (bb[3]*a3[0]);
    m[13]=(bb[0]*a0[1]) + (bb[1]*a1[1]) + (bb[2]*a2[1]) + (bb[3]*a3[1]);
    m[14]=(bb[0]*a0[2]) + (bb[1]*a1[2]) + (bb[2]*a2[2]) + (bb[3]*a3[2]);
    m[15]=(bb[0]*a0[3]) + (bb[1]*a1[3]) + (bb[2]*a2[3]) + (bb[3]*a3[3]);
*/
    
    __m128 A;
    __m128 B;
    __m128 R;
    
    for (int i=0;i<16;i+=4) {
        R=_mm_set_ps(0,0,0,0);
        
        
        for (int j=0;j<4;j++) {
            A=_mm_loadu_ps(a+(j*4));
            B=_mm_set_ps(b[i+j],b[i+j],b[i+j],b[i+j]);
            
            __m128 Q=_mm_mul_ps(A,B);
            R=_mm_add_ps(R,Q);
        }
        
        _mm_store_ps(m+i,R);
    }

}
