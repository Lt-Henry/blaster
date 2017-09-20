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

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include <blaster/constants.h>
#include <blaster/raster.h>
#include <blaster/color.h>
#include <blaster/vbo.h>
#include <blaster/vector.h>
#include <blaster/matrix.h>
#include <blaster/matrix_stack.h>
#include <blaster/tga.h>
#include <blaster/texture.h>



uint64_t get_ms(struct timespec start,struct timespec end) {
    uint64_t sec=((end.tv_sec - start.tv_sec) * 1000);
    uint64_t nsec=((end.tv_nsec - start.tv_nsec) / 1000000);

    return sec+nsec;
}

int main(int argc,char* argv[])
{

    int status;
    
    printf("blaster test tool\n");
    
    
    
    printf("\ntesting color module:\n\n");
    
    float c1[4];
    float c2[4];
    float c3[4];
    
    bl_color_set(c1,0.0f,1.0f,0.0f,1.0f);
    bl_color_set(c2,1.0f,0.0f,1.0f,0.0f);
    
    bl_color_add(c3,c1,c2);
    
    
    printf("bl_color_add: %.2f %.2f %.2f %.2f\n",c3[0],c3[1],c3[2],c3[3]);
    printf("bl_color_get_pixel: %X\n",bl_color_get_pixel(c1));


    float v1[4];
    float v2[4];
    float v3[4];
    
    float value;
    
    printf("\ntesting vector and matrix modules:\n\n");
    
    bl_vector_set(v1,0.0f,1.0f,0.0f,1.0f);
    bl_vector_set(v2,1.0f,0.0f,1.0f,0.0f);
    
    bl_vector_add(v3,v1,v2);
    
    printf("bl_vector_add: %.2f %.2f %.2f %.2f\n",v3[0],v3[1],v3[2],v3[3]);
    
    bl_vector_set(v1,2.0f,2.0f,2.0f,0.0f);
    bl_vector_normalize (v1);

    printf("bl_vector_normalize: %.2f %.2f %.2f %.2f\n",v1[0],v1[1],v1[2],v1[3]);
    
    value=bl_vector_norm(v1);

    printf("bl_vector_norm: %.2f\n",value);

    printf("perfomance test:");
    bl_vector_set(v1,4.0f,0.0f,0.0f,0.0f);
    
    float m1[16];
    float m2[16];
    float m3[16];
    
    bl_matrix_rotate_x(m1,1.0f);
    bl_matrix_rotate_y(m2,1.0f);
    
    struct timespec start,end;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    
    for (int n=0;n<0xffffff;n++) {
        bl_matrix_mult(m3,m1,m2);
        bl_vector_mult(v2,v1,m3);
    }
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    printf("%ld ms\n",get_ms(start,end));
    value=bl_vector_norm(v2);
    printf("distance: %.2f\n",value);
    
    printf("\ntesting raster:\n");
    
    bl_raster_t* raster = bl_raster_new(1024,768);
    
    //100 vertices with 12 attributes (pos,norm,color)
    bl_vbo_t* triangles = bl_vbo_new(100,12);
    
    //set background color (#839496)
    bl_color_from_ub(c1,0x83,0x94,0x96,0xff);
    bl_raster_set_clear_color(raster,c1);
    
    //clear
    bl_raster_clear(raster);
    
    //save output
    bl_tga_save(raster->color_buffer,"color.tga");
    
    bl_vbo_delete(triangles);
    bl_raster_delete(raster);

    return 0;
}
