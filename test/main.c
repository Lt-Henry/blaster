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


#include <stdio.h>
#include <blaster/color.h>
#include <blaster/math.h>
#include <blaster/tga.h>
#include <blaster/texture.h>

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


    float v1[4];
    float v2[4];
    float v3[4];
    
    float value;
    
    printf("\ntesting math module:\n\n");
    
    bl_vec_set(v1,0.0f,1.0f,0.0f,1.0f);
    bl_vec_set(v2,1.0f,0.0f,1.0f,0.0f);
    
    bl_vec_add(v3,v1,v2);
    
    printf("bl_vec_add: %.2f %.2f %.2f %.2f\n",v3[0],v3[1],v3[2],v3[3]);
    
    bl_vec_set(v1,2.0f,2.0f,2.0f,0.0f);
    bl_vec_normalize (v1);

    printf("bl_vec_normalize: %.2f %.2f %.2f %.2f\n",v1[0],v1[1],v1[2],v1[3]);
    
    value=bl_vec_norm(v1);

    printf("bl_vec_norm: %.2f\n",value);

    
    printf("\ntesting tga module:\n");

    bl_texture_t* texture;
    
    texture = bl_texture_new(800,600);
    
    bl_color_set(c1,1.0f,0.0f,0.0f,1.0f);
    
    for (int j=0;j<600;j++) {
        for (int i=0;i<800;i++) {
            float u = i/800.0f;
            float v = j/600.0f;
            
            c1[1]=u;
            c1[2]=v;
            
            uint32_t pixel = bl_color_get_pixel(c1);
            bl_texture_set_pixel(texture,i,j,pixel);
        }
    }
    
    bl_tga_save(texture,"out.tga");
    bl_texture_delete(texture);

    return 0;
}
