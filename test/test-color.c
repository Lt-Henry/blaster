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

int main(int argc,char* argv[])
{


    int status;
    
    printf("testing color module:\n\n");
    
    float c1[4];
    float c2[4];
    float c3[4];
    
    bl_color_set(c1,0.0f,1.0f,0.0f,1.0f);
    bl_color_set(c2,1.0f,0.0f,1.0f,0.0f);
    
    bl_color_add(c3,c1,c2);
    
    
    printf("bl_color_add: %.2f %.2f %.2f %.2f\n",c3[0],c3[1],c3[2],c3[3]);
    
    

    return 0;
}
