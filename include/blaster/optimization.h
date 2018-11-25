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

#ifndef BLASTER_OPTIMIZATION
#define BLASTER_OPTIMIZATION

/* Haswell optimizations */
#if defined(OPT_HASWELL)

#warning "Using Haswell optimizations"

#include <xmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>

#define BLASTER_RASTER_RCP_SSE
#define BLASTER_RASTER_CLEAR_SSE2
#define BLASTER_VECTOR_MULT_FMA
#define BLASTER_MATRIX_MULT_FMA

/* generic x64 optimizations */
#elif defined(OPT_AMD64)

#warning "Using basic amd64 optimizations"

#include <xmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>

#define BLASTER_RASTER_RCP_SSE
#define BLASTER_RASTER_CLEAR_SSE2
#define BLASTER_VECTOR_MULT_SSE
#define BLASTER_MATRIX_MULT_SSE

/* naive implementation */
#else

#define BLASTER_RASTER_RCP_GENERIC
#define BLASTER_RASTER_CLEAR_GENERIC
#define BLASTER_VECTOR_MULT_GENERIC
#define BLASTER_MATRIX_MULT_GENERIC

#endif

#endif
