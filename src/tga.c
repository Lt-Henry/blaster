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

#include <blaster/tga.h>
#include <stdio.h>
#include <stdint.h>

bl_texture_t* bl_tga_load(const char* filename)
{
    //ToDo
    return NULL;
}

void bl_tga_save(bl_texture_t* texture, const char* filename)
{



    FILE* file;
    
    file = fopen(filename,"w");
    
    bl_tga_header_t header;
    
    header.id_size=0;
    header.color_map_type=0;
    header.image_type=2;
    
    header.color_map_origin=0;
    header.color_map_length=0;
    header.color_map_entry=0;
    
    header.x_origin=0;
    header.y_origin=texture->height;
    header.width=texture->width;
    header.height=texture->height;
    
    header.bpp=32;
    header.descriptor=0;
    
    fwrite(&header,sizeof(bl_tga_header_t),1,file);
    
    int img_size=header.width*header.height;
    
    uint32_t* data = texture->data;
    
    for (int n=0;n<img_size;n++) {
        //data is stored as BGRA
        uint32_t pixel=data[n];
        
        uint32_t argb=  ((pixel & 0x000000ff)<<16) |
                        ((pixel & 0x0000ff00)) |
                        ((pixel & 0x00ff0000)>>16) |
                        ((pixel & 0xff000000));
                        
        
        fwrite(&argb,sizeof(uint32_t),1,file);
    }
    
    fclose(file);
}
