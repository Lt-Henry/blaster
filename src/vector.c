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

#include <blaster/vector.h>

#include <math.h>

#ifdef OPT_SSE

#include <xmmintrin.h>
#include <pmmintrin.h>

#endif

void bl_vector_set(float* v,float x,float y,float z,float w)
{
    v[0]=x;
    v[1]=y;
    v[2]=z;
    v[3]=w;
}

void bl_vector_copy(float* d,float* s)
{
    d[0]=s[0];
    d[1]=s[1];
    d[2]=s[2];
    d[3]=s[3];
}

void bl_vector_invert(float* v)
{
    v[0]=-v[0];
    v[1]=-v[1];
    v[2]=-v[2];
    v[3]=-v[3];
}

void bl_vector_add(float* r,float* a,float* b)
{
    r[0]=a[0]+b[0];
    r[1]=a[1]+b[1];
    r[2]=a[2]+b[2];
    r[3]=a[3]+b[3];
}

void bl_vector_sub(float* r,float* a,float* b)
{
    r[0]=a[0]-b[0];
    r[1]=a[1]-b[1];
    r[2]=a[2]-b[2];
    r[3]=a[3]-b[3];
}

float bl_vector_dot(const float* a,const float* b)
{
    float ret;
    ret=(a[0]*b[0]) + (a[1]*b[1]) + (a[2]*b[2]) + (a[3]*b[3]);

    return ret;
}

void bl_vector_cross(float* r,float* a,float* b)
{
    float tmp[4];

    tmp[0] = a[1]*b[2] - a[2]*b[1];
    tmp[1] = a[2]*b[0] - a[0]*b[2];
    tmp[2] = a[0]*b[1] - a[1]*b[0];
    tmp[3] = 0.0f;

    r[0]=tmp[0];
    r[1]=tmp[1];
    r[2]=tmp[2];
    r[3]=tmp[3];
}

float bl_vector_norm(const float* v)
{
    float tmp=(v[0]*v[0])+(v[1]*v[1])+(v[2]*v[2])+(v[3]*v[3]);
    return sqrtf(tmp);
}

void bl_vector_normalize(float* v)
{
    float rW=1.0f/bl_vector_norm(v);

    v[0]*=rW;
    v[1]*=rW;
    v[2]*=rW;
    v[3]*=rW;
}

void bl_vector_homogeneus(float* v)
{
    float rW=1.0f/v[3];

    v[0]*=rW;
    v[1]*=rW;
    v[2]*=rW;
    v[3]=1.0f;
}

void bl_vector_mult(float* r,const float* v,const float* m)
{

#ifdef OPT_SSE
    
    __m128 V;
    __m128 M;
    __m128 R;
    __m128 Q;
    
    
    //R=_mm_set_ps(0.0f,0.0f,0.0f,0.0f);
    V=_mm_load_ps1(v);
    M=_mm_loadu_ps(m);
    R=_mm_mul_ps(V,M);
        
    for (size_t i=1;i<4;i++) {
        
        size_t j=i<<2;
        
        V=_mm_load_ps1(v+i);
        M=_mm_loadu_ps(m+j);
        Q=_mm_mul_ps(V,M);
        R=_mm_add_ps(R,Q);
    }
    
    _mm_store_ps(r,R);

#else

    float w[4]={v[0],v[1],v[2],v[3]};

    r[0]= (m[4*0+0] * w[0]) + (m[4*1+0] * w[1]) + (m[4*2+0] * w[2]) + (m[4*3+0] * w[3]);
    r[1]= (m[4*0+1] * w[0]) + (m[4*1+1] * w[1]) + (m[4*2+1] * w[2]) + (m[4*3+1] * w[3]);
    r[2]= (m[4*0+2] * w[0]) + (m[4*1+2] * w[1]) + (m[4*2+2] * w[2]) + (m[4*3+2] * w[3]);
    r[3]= (m[4*0+3] * w[0]) + (m[4*1+3] * w[1]) + (m[4*2+3] * w[2]) + (m[4*3+3] * w[3]);
    
#endif
}

void bl_vector_scale(float* v,float s)
{
    v[0]=v[0]*s;
    v[1]=v[1]*s;
    v[2]=v[2]*s;
    v[3]=v[3]*s;
}
