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
#include <blaster/color.h>


void bl_color_set(bl_color_t* c,float r,float g,float b,float a)
{
    c->r=r;
    c->g=g;
    c->b=b;
    c->a=a;
}

void bl_color_add(bl_color_t* c,bl_color_t* a,bl_color_t* b)
{
    c->r = a->r + b->r;
    c->g = a->g + b->g;
    c->b = a->b + b->b;
    c->a = a->a + b->a;
}

void bl_color_sub(bl_color_t* c,bl_color_t* a,bl_color_t* b)
{
    c->r = a->r - b->r;
    c->g = a->g - b->g;
    c->b = a->b - b->b;
    c->a = a->a - b->a;
}

void bl_color_scale(bl_color_t* c,float s)
{
    c->r*=s;
    c->g*=s;
    c->b*=s;
    c->a*=s;
}

void bl_color_mult(bl_color_t* c,bl_color_t* a,bl_color_t* b)
{
    c->r = a->r * b->r;
    c->g = a->g * b->g;
    c->b = a->b * b->b;
    c->a = a->a * b->a;
}

void bl_color_clamp(bl_color_t* c)
{
    c->r=(c->r>1.0f) ? 1.0f : c->r;
    c->g=(c->g>1.0f) ? 1.0f : c->g;
    c->b=(c->b>1.0f) ? 1.0f : c->b;
    c->a=(c->a>1.0f) ? 1.0f : c->a;
    
    c->r=(c->r<0.0f) ? 0.0f : c->r;
    c->g=(c->g<0.0f) ? 0.0f : c->g;
    c->b=(c->b<0.0f) ? 0.0f : c->b;
    c->a=(c->a<0.0f) ? 0.0f : c->a;
    
}

bl_pixel_t bl_color_get_pixel(const bl_color_t* c)
{

#ifdef BLASTER_COLOR_GET_PIXEL_SSE

    bl_pixel_t pixel;
    uint32_t ci[4];
    
    __m128i CI;
    __m128 C;
    __m128 K;
    
    C = _mm_loadu_ps(c->channel); // 1c
    K = _mm_set_ps1(255.0f); // 1c
    C = _mm_mul_ps(C,K); // 3c
    
    CI = _mm_cvtps_epi32(C); // 3c

    __m128i C1 = _mm_bsrli_si128(CI,3); // 1c
    __m128i C2 = _mm_bsrli_si128(CI,6); // 1c
    __m128i C3 = _mm_bsrli_si128(CI,9); // 1c

    CI = _mm_or_si128(CI,C1); // 1c
    CI = _mm_or_si128(CI,C2); // 1c
    CI = _mm_or_si128(CI,C3); // 1c

    _mm_storeu_si128((__m128i*)ci,CI); // 1c
    pixel.value=ci[0]; // 1c
    
    return pixel;
#endif

#ifdef BLASTER_COLOR_GET_PIXEL_GENERIC

    bl_pixel_t pixel;
    
    pixel.channel[0] = c->r*255.0f;
    pixel.channel[1] = c->g*255.0f;
    pixel.channel[2] = c->b*255.0f;
    pixel.channel[3] = c->a*255.0f;
    
    return pixel;

#endif
}

void bl_color_from_pixel(bl_color_t* c,bl_pixel_t pixel)
{
    const float rc=1.0f/255.0f;

    c->r=pixel.channel[0];
    c->g=pixel.channel[1];
    c->b=pixel.channel[2];
    c->a=pixel.channel[3];
    
    c->r*=rc;
    c->g*=rc;
    c->b*=rc;
    c->a*=rc;
    
}

void bl_color_from_ub(bl_color_t* c,uint8_t r,uint8_t g,uint8_t b,uint8_t a)
{
    const float rc=1.0f/255.0f;
    c->r=r;
    c->g=g;
    c->b=b;
    c->a=a;
    
    c->r*=rc;
    c->g*=rc;
    c->b*=rc;
    c->a*=rc;
}
