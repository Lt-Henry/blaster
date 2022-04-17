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
#include <blaster/constants.h>

#include <stdio.h>
#include <stdint.h>

bl_texture_t* bl_tga_load(const char* filename)
{
    bl_texture_t* texture = NULL;
    
    FILE* file;
    
    file = fopen(filename,"rb");
    
    bl_tga_header_t header;
    fread(&header,sizeof(bl_tga_header_t),1,file);
    printf("tga type:%d\n",header.image_type);
    printf("tga size:%dx%d\n",header.width,header.height);
    printf("tga bpp:%d\n",header.bpp);
    
    size_t bitmap_size = header.width*header.height*(header.bpp/8);
    printf("tga data size:%d\n",bitmap_size);
    
    switch (header.image_type) {
        case  BL_TGA_TYPE_UNCOMPRESSED_RGB: {
            uint8_t* data=malloc(bitmap_size);

            for (int n=0;n<header.width*header.height;n++) {
                uint8_t tmp[4];
                fread(tmp,header.bpp/8,1,file);

                data[0]=tmp[0];
                data[1]=tmp[1];
                data[2]=tmp[2];
                data[3]=tmp[3];

                data+=4;
            }

            texture = bl_texture_new_from_data(header.width,header.height,BL_TEXTURE_U32,data);
            free(data);
        }
        break;

        case BL_TGA_TYPE_RLE_RGB: {
            uint8_t* data=malloc(bitmap_size);

            size_t remain = bitmap_size;
            int section = 0; //0 header, 1 rle value, 2 rle chunk, 3 raw chunk
            int chunk;
            uint8_t tmp[4];

            while(remain>0) {
                switch (section) {
                    case 0:
                        fread(&chunk,1,1,file);
                        if (chunk<128) {
                            section = 3;
                            chunk = chunk + 1;
                        }
                        else {
                            section = 1;
                            chunk = chunk - 127;
                        }
                    break;

                    case 1:
                        fread(tmp,header.bpp/8,1,file);

                        data[0]=tmp[0];
                        data[1]=tmp[1];
                        data[2]=tmp[2];
                        data[3]=tmp[3];

                        section = 2;
                        data+=4;
                        remain-=4;
                        chunk--;

                        if (chunk == 0) {
                            section = 0;
                        }
                    break;

                    case 2:
                        data[0]=tmp[0];
                        data[1]=tmp[1];
                        data[2]=tmp[2];
                        data[3]=tmp[3];

                        data+=4;
                        remain-=4;

                        chunk--;
                        if (chunk == 0) {
                            section = 0;
                        }
                    break;

                    case 3:
                        fread(tmp,header.bpp/8,1,file);

                        data[0]=tmp[0];
                        data[1]=tmp[1];
                        data[2]=tmp[2];
                        data[3]=tmp[3];

                        data+=4;
                        remain-=4;

                        chunk--;
                        if (chunk == 0) {
                            section = 0;
                        }
                    break;
                }
            }

            free(data);
        }
        break;

    }
    fclose(file);
    
    return texture;
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
