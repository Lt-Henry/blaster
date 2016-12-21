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

#include <blaster/texturepool.h>

#include <stdlib.h>
#include <string>


static void bl_texturepool_free_node(bl_texturepool_node_t* node)
{
	if (node!=NULL) {
		free(node->name);
		bl_texture_free(node->texture);
		bl_texturepool_free_node(node->left);
		bl_texturepool_free_node(node->right);
		free(node);
	}
}

static int bl_texturepool_find(const char* name,
								bl_texturepool_node_t* node,
								bl_texturepool_node_t** target )
{
	if (node!=NULL) {
		int cmp=strcmp(name,node->name);
		
		// found
		if (cmp==0) {
			*target=node;
				return 0;
		}
		
		// left
		if (cmp<0) {
			if (node->left==NULL) {
				*target=node;
				return -1;
			}
			else {
				return bl_texturepool_find(name,node->left,target);
			}
			
		}
		
		// right
		if (cmp>0) {
			if (node->right==NULL) {
				*target=node;
				return 1;
			}
			else {
				return bl_texturepool_find(name,node->right,target);
			}

		}
	}
}

static bl_texturepool_node_t* bl_texturepool_create_node(const char* name,
												const bl_texture_t* texture
)
{

	bl_texturepool_node_t* node = NULL;
	
	node = malloc(sizeof(bl_texturepool_node_t));
	node->name=strdup(name);
	node->texture=texture;
	node->right=NULL;
	node->left=NULL;

	return node;
}

bl_texturepool_t* bl_texturepool_new(bl_texturepool_load_cb_t load_cb)
{
	bl_texturepool_t* pool = NULL;
	
	pool = malloc(sizeof(bl_texturepool_t));
	
	pool->load_cb=load_cb;
	pool->root=NULL;
}

void bl_texturepool_free(bl_texturepool_t* pool)
{
	bl_texturepool_free_node(pool->root);
	free(pool);
}

bl_texture_t* bl_texturepool_get(bl_texturepool_t* pool,const char* name)
{
	if (pool->root==NULL) {
		
		bl_texture_t* texture = pool->load_cb(name);
		pool->root=bl_texturepool_create_node(name,texture);
		
	}
	else {
	
		bl_texturepool_node_t* target;
		int cmp = bl_texturepool_find(name,pool->root,&target);
		
		// found
		if (cmp==0) {
			return target->texture;
		}
		else {
		
			bl_texture_t* texture = pool->load_cb(name);
			bl_texturepool_node_t* node=bl_texturepool_create_node(name,texture);

			// fit in left
			if (cmp<0) {
				target->left=node;
			}
		
			// fit in right
			if (cmp>0) {
				target->right=node;
			}
			
			return texture;
		}
	}
}

