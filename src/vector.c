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
#include <blaster/vector.h>

#include <math.h>



void bl_vector_set(bl_vector_t* v,float x,float y,float z,float w)
{
    v->x=x;
    v->y=y;
    v->z=z;
    v->w=w;
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

#ifdef BLASTER_VECTOR_MULT_FMA
void bl_vector_mult(bl_vector_t* r,const bl_vector_t* v,const bl_matrix_t* m)
{
    
    __m128 V;
    __m128 M;
    __m128 R;
    __m128 Q;
    
    
    //R=_mm_set_ps(0.0f,0.0f,0.0f,0.0f);
    V=_mm_load_ps1(v->data);
    M=_mm_loadu_ps(m->data);
    R=_mm_mul_ps(V,M);
        
    for (size_t i=1;i<4;i++) {
        
        size_t j=i<<2;
        
        V=_mm_load_ps1(v->data+i);
        M=_mm_loadu_ps(m->data+j);
        R=_mm_fmadd_ps(V,M,R);
    }
    
    _mm_store_ps(r->data,R);

}
#endif

#ifdef BLASTER_VECTOR_MULT_SSE
void bl_vector_mult(bl_vector_t* r,const bl_vector_t* v,const bl_matrix_t* m)
{
    
    __m128 V;
    __m128 M;
    __m128 R;
    __m128 Q;
    
    
    //R=_mm_set_ps(0.0f,0.0f,0.0f,0.0f);
    V=_mm_load_ps1(v->data);
    M=_mm_loadu_ps(m->data);
    R=_mm_mul_ps(V,M);
        
    for (size_t i=1;i<4;i++) {
        
        size_t j=i<<2;
        
        V=_mm_load_ps1(v->data+i);
        M=_mm_loadu_ps(m->data+j);
        Q=_mm_mul_ps(V,M);
        R=_mm_add_ps(R,Q);
    }
    
    _mm_store_ps(r->data,R);

}
#endif

#ifdef BLASTER_VECTOR_MULT_GENERIC
void bl_vector_mult(bl_vector_t* r,const bl_vector_t* v,const bl_matrix_t* m)
{
    r->x= (m->data[4*0+0] * v->x) + (m->data[4*1+0] * v->y) + (m->data[4*2+0] * v->z) + (m->data[4*3+0] * v->w);
    r->y= (m->data[4*0+1] * v->x) + (m->data[4*1+1] * v->y) + (m->data[4*2+1] * v->z) + (m->data[4*3+1] * v->w);
    r->z= (m->data[4*0+2] * v->x) + (m->data[4*1+2] * v->y) + (m->data[4*2+2] * v->z) + (m->data[4*3+2] * v->w);
    r->w= (m->data[4*0+3] * v->x) + (m->data[4*1+3] * v->y) + (m->data[4*2+3] * v->z) + (m->data[4*3+3] * v->w);
    
}
#endif

void bl_vector_scale(bl_vector_t* v,float s)
{
    v->x*=s;
    v->y*=s;
    v->z*=s;
    v->w*=s;
}
