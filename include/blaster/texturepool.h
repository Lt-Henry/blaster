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

#ifndef _BLASTER_TEXTUREPOOL_
#define _BLASTER_TEXTUREPOOL_

#include "texture.h"

/*!
	Internal binary tree node
*/
typedef struct {
	
	struct bl_texturepool_node_t* left;
	struct bl_texturepool_node_t* right;
	
	char* name;
	bl_texture_t* texture;
	
} bl_texturepool_node_t;

/*!
	Load texture file callback
*/
typedef bl_texture_t* (*bl_texturepool_load_cb_t)(const char*);

/*!
	Texture pool data structure
*/
typedef struct {
	
	bl_texturepool_load_cb_t load_cb;
	bl_texturepool_node_t* root;
	
} bl_texturepool_t;

/*!
	Creates a new texturepool
*/
bl_texturepool_t* bl_texturepool_new(bl_texturepool_load_cb_t load_cb);

/*!
	Free the texturepool
*/
void bl_texturepool_free(bl_texturepool_t* pool);

/*!
	Gets a pointer to texture given its filename as key, if not found, tries 
	to it load from disk.
*/
bl_texture_t* bl_texturepool_get(bl_texturepool_t* pool,const char* name);

#endif
