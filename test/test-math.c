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
#include <blaster/math.h>


int main(int argc,char* argv[])
{
    float v1[4];
    float v2[4];
    float v3[4];
    
    printf("testing math module:\n\n");
    
    bl_vec_set(v1,0.0f,1.0f,0.0f,1.0f);
    bl_vec_set(v2,1.0f,0.0f,1.0f,0.0f);
    
    bl_vec_add(v3,v1,v2);
    
    printf("bl_vec_add: %.2f %.2f %.2f %.2f\n",v3[0],v3[1],v3[2],v3[3]);
    
    return 0;
}
