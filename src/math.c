/*
	blaster
	
	Copyright (C) 2016  Enrique Medina Gremaldos <quiqueiii@gmail.com>

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

#include <blaster/math.h>

#include <math.h>

void bl_vec_set(float* v,float x,float y,float z,float w)
{
	v[0]=x;
	v[1]=y;
	v[2]=z;
	v[3]=w;
}

void bl_vec_copy(float* d,float* s)
{
	d[0]=s[0];
	d[1]=s[1];
	d[2]=s[2];
	d[3]=s[3];
}

void bl_vec_add(float* r,float* a,float* b)
{
	r[0]=a[0]+b[0];
	r[1]=a[1]+b[1];
	r[2]=a[2]+b[2];
	r[3]=a[3]+b[3];
}

void bl_vec_sub(float* r,float* a,float* b)
{
	r[0]=a[0]-b[0];
	r[1]=a[1]-b[1];
	r[2]=a[2]-b[2];
	r[3]=a[3]-b[3];
}

float bl_vec_dot(const float* a,const float* b)
{
	float ret;
	ret=(a[0]*b[0]) + (a[1]*b[1]) + (a[2]*b[2]) + (a[3]*b[3]);
	
	return ret;
}

void bl_vec_cross(float* r,float* a,float* b)
{
	float tmp[4];
	
	tmp[0] = a[1]*b[2] - a[2]*b[1];
	tmp[1] = a[2]*b[0] - a[0]*b[2];
	tmp[2] = a[0]*b[1] - a[1]*b[0];
	tmp[3] = 0.0f;
	
	r[0]=tmp[0];
	r[1]=tmp[1];
	r[2]=tmp[2];
	r[3]=tmp[3];
}

float bl_vec_norm(const float* v)
{
	float tmp=(v[0]*v[0])+(v[1]*v[1])+(v[2]*v[2])+(v[3]*v[3]);
	return sqrtf(tmp);
}

void bl_vec_normalize(float* v)
{
	float rW=1.0f/bl_vec_norm(v);
	
	v[0]*=rW;
	v[1]*=rW;
	v[2]*=rW;
	v[3]*=rW;
}

void bl_vec_homogeneus(float* v)
{
	float rW=1.0f/v[3];
	
	v[0]*=rW;
	v[1]*=rW;
	v[2]*=rW;
	v[3]=1.0f;
}

void bl_vec_mult(float* r,const float* v,const float* m)
{
	float w[4]={v[0],v[1],v[2],v[3]};
	
	r[0]= (m[4*0+0] * w[0]) + (m[4*1+0] * w[1]) + (m[4*2+0] * w[2]) + (m[4*3+0] * w[3]);
	r[1]= (m[4*0+1] * w[0]) + (m[4*1+1] * w[1]) + (m[4*2+1] * w[2]) + (m[4*3+1] * w[3]);
	r[2]= (m[4*0+2] * w[0]) + (m[4*1+2] * w[1]) + (m[4*2+2] * w[2]) + (m[4*3+2] * w[3]);
	r[3]= (m[4*0+3] * w[0]) + (m[4*1+3] * w[1]) + (m[4*2+3] * w[2]) + (m[4*3+3] * w[3]);
}

void bl_mat_copy(float* d,float* s)
{

	for (int n=0;n<16;n++) {
		d[n]=s[n];
	}
}

void bl_mat_identity(float* m)
{
	m[0]=1.0f;
	m[1]=0.0f;
	m[2]=0.0f;
	m[3]=0.0f;
	
	m[4]=0.0f;
	m[5]=1.0f;
	m[6]=0.0f;
	m[7]=0.0f;
	
	m[8]=0.0f;
	m[9]=0.0f;
	m[10]=1.0f;
	m[11]=0.0f;
	
	m[12]=0.0f;
	m[13]=0.0f;
	m[14]=0.0f;
	m[15]=1.0f;
}

void bl_mat_translation(float* m,float x,float y,float z)
{
	m[0]=1.0f;
	m[1]=0.0f;
	m[2]=0.0f;
	m[3]=0.0f;
	
	m[4]=0.0f;
	m[5]=1.0f;
	m[6]=0.0f;
	m[7]=0.0f;
	
	m[8]=0.0f;
	m[9]=0.0f;
	m[10]=1.0f;
	m[11]=0.0f;
	
	m[12]=x;
	m[13]=y;
	m[14]=z;
	m[15]=1.0f;
}

void bl_mat_rotation_x(float* m,float rads)
{
	m[0]=1.0f;
	m[1]=0.0f;
	m[2]=0.0f;
	m[3]=0.0f;
	
	m[4]=0.0f;
	m[5]=cos(rads);
	m[6]=sin(rads);
	m[7]=0.0f;
	
	m[8]=0.0f;
	m[9]=-sin(rads);
	m[10]=cos(rads);
	m[11]=0.0f;
	
	m[12]=0.0f;
	m[13]=0.0f;
	m[14]=0.0f;
	m[15]=1.0f;
}

void bl_mat_rotation_y(float* m,float rads)
{
	m[0]=cos(rads);
	m[1]=0.0f;
	m[2]-=sin(rads);
	m[3]=0.0f;
	
	m[4]=0.0f;
	m[5]=1.0f;
	m[6]=0.0f;
	m[7]=0.0f;
	
	m[8]=sin(rads);
	m[9]=0.0f;
	m[10]=cos(rads);
	m[11]=0.0f;
	
	m[12]=0.0f;
	m[13]=0.0f;
	m[14]=0.0f;
	m[15]=1.0f;
}

void bl_mat_rotation_z(float* m,float rads)
{
	m[0]=cos(rads);
	m[1]=sin(rads);
	m[2]=0.0f;
	m[3]=0.0f;
	
	m[4]=-sin(rads);
	m[5]=cos(rads);
	m[6]=0.0f;
	m[7]=0.0f;
	
	m[8]=0.0f;
	m[9]=0.0f;
	m[10]=1.0f;
	m[11]=0.0f;
	
	m[12]=0.0f;
	m[13]=0.0f;
	m[14]=0.0f;
	m[15]=1.0f;
}

void bl_mat_mult(float* m,float* a,float* b)
{

	for (int i=0;i<16;i+=4) {
		for (int j=0;j<4;j++) {
			m[i + j] = (b[i + 0] * a[j + 0])
			+ (b[i + 1] * a[j + 4])
			+ (b[i + 2] * a[j + 8])
			+ (b[i + 3] * a[j + 12]);
		}
	}
}

