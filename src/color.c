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


#include <blaster/color.h>


void bl_color_set(float* c,float r,float g,float b,float a)
{
    c[0]=r;
    c[1]=g;
    c[2]=b;
    c[3]=a;
}

void bl_color_add(float* c,float* a,float* b)
{
    c[0]=a[0]+b[0];
    c[1]=a[1]+b[1];
    c[2]=a[2]+b[2];
    c[3]=a[3]+b[3];
}

void bl_color_sub(float* c,float* a,float* b)
{
    c[0]=a[0]-b[0];
    c[1]=a[1]-b[1];
    c[2]=a[2]-b[2];
    c[3]=a[3]-b[3];
}

void bl_color_scale(float* c,float s)
{
    c[0]=c[0]*s;
    c[1]=c[1]*s;
    c[2]=c[2]*s;
    c[3]=c[3]*s;
}

void bl_color_mult(float* c,float* a,float* b)
{
    c[0]=a[0]*b[0];
    c[1]=a[1]*b[1];
    c[2]=a[2]*b[2];
    c[3]=a[3]*b[3];
}

void bl_color_clamp(float* c)
{
    c[0]=(c[0]>1.0f) ? 1.0f : c[0];
    c[1]=(c[1]>1.0f) ? 1.0f : c[1];
    c[2]=(c[2]>1.0f) ? 1.0f : c[2];
    c[3]=(c[3]>1.0f) ? 1.0f : c[3];

    c[0]=(c[0]<0.0f) ? 0.0f : c[0];
    c[1]=(c[1]<0.0f) ? 0.0f : c[1];
    c[2]=(c[2]<0.0f) ? 0.0f : c[2];
    c[3]=(c[3]<0.0f) ? 0.0f : c[3];
}

uint32_t bl_color_get_pixel(const float* c)
{
    uint32_t pixel;

    uint32_t ci[4];

    ci[0]=c[0]*255.0f;
    ci[1]=c[1]*255.0f;
    ci[2]=c[2]*255.0f;
    ci[3]=c[3]*255.0f;

    // (msb) ABGR (lsb)
    pixel=(ci[3]<<24) | (ci[2]<<16) | (ci[1]<<8) | ci[0];

    return pixel;
}

void bl_color_from_pixel(float* c,uint32_t pixel)
{
    uint32_t ci[4];

    // ToDo: check
    
    ci[0]=(pixel & 0xff000000)>>24;
    ci[1]=(pixel & 0x00ff0000)>>16;
    ci[2]=(pixel & 0x0000ff00)>>8;
    ci[3]=(pixel & 0x000000ff);

    float rc=1.0f/255.0f;

    c[0]=(float)ci[0];
    c[1]=(float)ci[1];
    c[2]=(float)ci[2];
    c[3]=(float)ci[3];

    c[0]*=rc;
    c[1]*=rc;
    c[2]*=rc;
    c[3]*=rc;
}

void bl_color_from_ub(float* c,uint8_t r,uint8_t g,uint8_t b,uint8_t a)
{
    c[0]=r/255.0f;
    c[1]=g/255.0f;
    c[2]=b/255.0f;
    c[3]=a/255.0f;
}
