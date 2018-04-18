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

void bl_vector_copy(bl_vector_t* d,bl_vector_t* s)
{
    d->data[0]=s->data[0];
    d->data[1]=s->data[1];
    d->data[2]=s->data[2];
    d->data[3]=s->data[3];
    
}

void bl_vector_invert(bl_vector_t* v)
{
    v->x=-v->x;
    v->y=-v->y;
    v->z=-v->z;
    v->w=-v->w;
}

void bl_vector_add(bl_vector_t* r,bl_vector_t* a,bl_vector_t* b)
{
    r->x=a->x+b->x;
    r->y=a->y+b->y;
    r->z=a->z+b->z;
    r->w=a->w+b->w;
}

void bl_vector_sub(bl_vector_t* r,bl_vector_t* a,bl_vector_t* b)
{
    r->x=a->x-b->x;
    r->y=a->y-b->y;
    r->z=a->z-b->z;
    r->w=a->w-b->w;

}

float bl_vector_dot(const bl_vector_t* a,const bl_vector_t* b)
{
    float ret;
    ret=(a->x*b->x) + (a->y*b->y) + (a->z*b->z) + (a->w*b->w);

    return ret;
}

void bl_vector_cross(bl_vector_t* r,bl_vector_t* a,bl_vector_t* b)
{
    float tmp[4];

    tmp[0] = a->y*b->z - a->z*b->y;
    tmp[1] = a->z*b->x - a->x*b->z;
    tmp[2] = a->x*b->y - a->y*b->x;
    tmp[3] = 0.0f;

    r->x=tmp[0];
    r->y=tmp[1];
    r->z=tmp[2];
    r->w=tmp[3];
}

float bl_vector_norm(const bl_vector_t* v)
{
    float tmp=(v->x*v->x)+(v->y*v->y)+(v->z*v->z)+(v->w*v->w);
    return sqrtf(tmp);
}

void bl_vector_normalize(bl_vector_t* v)
{
    float rW=1.0f/bl_vector_norm(v);

    v->x*=rW;
    v->y*=rW;
    v->z*=rW;
    v->w*=rW;
}

void bl_vector_homogeneus(bl_vector_t* v)
{
    float rW=1.0f/v->w;

    v->x*=rW;
    v->y*=rW;
    v->z*=rW;
    v->w=1.0f;
}

void bl_vector_mult(bl_vector_t* r,const bl_vector_t* v,const bl_matrix_t* m)
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

    float w[4]={v->x,v->y,v->z,v->w};

    r->x= (m[4*0+0] * w[0]) + (m[4*1+0] * w[1]) + (m[4*2+0] * w[2]) + (m[4*3+0] * w[3]);
    r->y= (m[4*0+1] * w[0]) + (m[4*1+1] * w[1]) + (m[4*2+1] * w[2]) + (m[4*3+1] * w[3]);
    r->z= (m[4*0+2] * w[0]) + (m[4*1+2] * w[1]) + (m[4*2+2] * w[2]) + (m[4*3+2] * w[3]);
    r->w= (m[4*0+3] * w[0]) + (m[4*1+3] * w[1]) + (m[4*2+3] * w[2]) + (m[4*3+3] * w[3]);
    
#endif
}

void bl_vector_scale(bl_vector_t* v,float s)
{
    v->x*=s;
    v->y*=s;
    v->z*=s;
    v->w*=s;
}
