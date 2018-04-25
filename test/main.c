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
#include <math.h>

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
    
    bl_color_t c1;
    bl_color_t c2;
    bl_color_t c3;
    
    bl_color_set(&c1,0.0f,1.0f,0.0f,1.0f);
    bl_color_set(&c2,1.0f,0.0f,1.0f,0.0f);
    
    bl_color_add(&c3,&c1,&c2);
    
    
    printf("bl_color_add: %.2f %.2f %.2f %.2f\n",c3.r,c3.g,c3.b,c3.a);
    printf("bl_color_get_pixel: %X\n",bl_color_get_pixel(&c1));


    bl_vector_t v1;
    bl_vector_t v2;
    bl_vector_t v3;
    
    float value;
    
    printf("\ntesting vector and matrix modules:\n\n");
    
    bl_vector_set(&v1,0.0f,1.0f,0.0f,1.0f);
    bl_vector_set(&v2,1.0f,0.0f,1.0f,0.0f);
    
    bl_vector_add(&v3,&v1,&v2);
    
    printf("bl_vector_add: %.2f %.2f %.2f %.2f\n",v3.x,v3.y,v3.z,v3.w);
    
    bl_vector_set(&v1,2.0f,2.0f,2.0f,0.0f);
    bl_vector_normalize(&v1);

    printf("bl_vector_normalize: %.2f %.2f %.2f %.2f\n",v1.x,v1.y,v1.z,v1.w);
    
    value=bl_vector_norm(&v1);

    printf("bl_vector_norm: %.2f\n",value);

    
    bl_vector_set(&v1,4.0f,0.0f,0.0f,0.0f);
    
    bl_matrix_t m1;
    bl_matrix_t m2;
    bl_matrix_t m3;
    
    bl_matrix_rotate_x(&m1,1.0f);
    bl_matrix_rotate_y(&m2,1.0f);
    bl_matrix_identity(&m3);
    
    
    printf("perfomance test:");
    
    struct timespec start,end;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    
    for (int n=0;n<0x2ffffff;n++) {
        bl_matrix_mult(&m3,&m1,&m2);
        bl_vector_mult(&v2,&v1,&m3);
    }
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    printf("%ld ms\n",get_ms(start,end));
    value=bl_vector_norm(&v2);
    
    
    printf("v2: %.2f %.2f %.2f %.2f\n",v2.x,v2.y,v2.z,v2.w);
    printf("distance: %.2f\n",value);
    
    printf("m1:\n");
    bl_matrix_print(&m1);
    printf("m2:\n");
    bl_matrix_print(&m2);
    printf("m3:\n");
    bl_matrix_print(&m3);
    
    printf("\ntesting raster:\n");
    
    bl_raster_t* raster = bl_raster_new(1024,768);
    
    //1000 vertices with 8 attributes (pos,color)
    uint8_t attribs[16] = { BL_F32,4,
                            BL_F32,4,
                            0,0,0,0,0,0,0,0,0,0,0,0 };
                            
    bl_vbo_t* points = bl_vbo_new(1000,attribs);
    
    double alpha=0.0;
    float step=0.0f;
    for (int n=0;n<1000;n++) {
        float pos[4];
        pos[0] = cos(alpha);
        pos[1] = sin(alpha);
        pos[2]= -(10.0f+step);
        pos[3]=1.0f;
        
        float color[4]={1.0f,0.0f,0.0f,1.0f};
        
        alpha+=0.1;
        step+=0.1f;
        
        bl_vbo_set(points,0,n,pos);
        bl_vbo_set(points,1,n,color);
    }
    
    
    //set background color (#839496)
    bl_color_from_ub(c1,0x83,0x94,0x96,0xff);
    bl_raster_set_clear_color(raster,c1);
    
    //clear
    bl_raster_clear(raster);
    
    bl_matrix_stack_load_identity(raster->modelview);
    bl_matrix_stack_load_identity(raster->projection);
    bl_matrix_stack_frustum(raster->projection,-1,1,-1,1,10,100);
    
    bl_raster_draw_points(raster,points);
    
    
    //save output
    bl_tga_save(raster->color_buffer,"color.tga");
    
    bl_vbo_delete(points);
    bl_raster_delete(raster);

    return 0;
}
