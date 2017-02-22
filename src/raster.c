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

#include <blaster/raster.h>
#include <blaster/constants.h>

#include <stdio.h>
#include <stdlib.h>


static void bl_raster_clean_tiles(bl_raster_t* raster)
{
	int N=raster->tiles_width*raster->tiles_height;
	
	for (int n=0;n<N;n++) {
		bl_tile_delete(raster->tiles[n]);
	}

	free(raster->tiles);

}

static void bl_raster_cmd_queue(bl_raster_t* raster,bl_command_t cmd)
{
	if (raster->cmd_size<BL_CMD_SIZE) {
	
		raster->cmd_size++;
		raster->cmd_end=(raster->cmd_end+1)%BL_CMD_SIZE;
		raster->cmd_queue[raster->cmd_end]=cmd;
	}
}

static bl_command_t bl_raster_cmd_dequeue(bl_raster_t* raster)
{

	bl_command_t cmd;
	cmd.type=BL_CMD_NONE;
	
	if (raster->cmd_size>0) {
	
		raster->cmd_size--;
		cmd=raster->cmd_queue[raster->cmd_begin];
		raster->cmd_begin=(raster->cmd_begin+1)%BL_CMD_SIZE;
	}
	
	return cmd;
}

bl_raster_t* bl_raster_new()
{
	bl_raster_t* raster;
	
	raster=malloc(sizeof(bl_raster_t));
	
	raster->tiles_width=0;
	raster->tiles_height=0;
	raster->tiles=NULL;
	
	raster->cmd_queue=malloc(sizeof(bl_command_t)*BL_CMD_SIZE);
	raster->cmd_begin=1;
	raster->cmd_end=0;
	raster->cmd_size=0;
	
	return raster;
}

void bl_raster_delete(bl_raster_t* raster)
{
	free(raster->cmd_queue);
	
	bl_raster_clean_tiles(raster);
	free(raster);
}

void bl_raster_resize(bl_raster_t* raster,int width,int height)
{
	bl_raster_clean_tiles(raster);
	
	// compute number of tiles and real size (which may be smaller than requested)
	raster->tiles_width=width/BL_TILE_SIZE;
	raster->tiles_height=height/BL_TILE_SIZE;
	
	raster->screen_width=raster->tiles_width*BL_TILE_SIZE;
	raster->screen_height=raster->tiles_height*BL_TILE_SIZE;
	
	raster->tiles=malloc(sizeof(bl_tile_t*)*raster->tiles_width*raster->tiles_height);
	
	for (int j=0;j<raster->tiles_height;j++) {
		for (int i=0;i<raster->tiles_width;i++) {
			int index = i+j*raster->tiles_width;
			
			int x=i*BL_TILE_SIZE;
			int y=j*BL_TILE_SIZE;

			raster->tiles[index]=bl_tile_new(x,y,BL_TILE_SIZE,BL_TILE_SIZE);
		}
	}
}

void bl_raster_set_clear_color(bl_raster_t* raster,uint32_t color)
{
	bl_command_t cmd;
	
	cmd.type=BL_CMD_CLEAR_COLOR;
	cmd.clear_color.color=color;
	
	bl_raster_cmd_queue(raster,cmd);
}

void bl_raster_clear(bl_raster_t* raster)
{
	bl_command_t cmd;
	
	cmd.type=BL_CMD_CLEAR;
	
	int num_tiles=raster->tiles_width*raster->tiles_height;
	
	for (int n=0;n<num_tiles;n++) {
		cmd.clear.tile=raster->tiles[n];
		bl_raster_cmd_queue(raster,cmd);
	}
}

void bl_raster_update(bl_raster_t* raster)
{
	while (raster->cmd_size>0) {
		bl_command_t cmd = bl_raster_cmd_dequeue(raster);
		
		switch (cmd.type) {
			case BL_CMD_CLEAR:
				bl_tile_clear(cmd.clear.tile,0xff0000ff,0xffff);
			break;
		}
	}
}

int bl_raster_get_width(bl_raster_t* raster)
{
	return raster->screen_width;
}

int bl_raster_get_height(bl_raster_t* raster)
{
	return raster->screen_height;
}

