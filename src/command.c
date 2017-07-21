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

#include <blaster/command.h>

bl_command_buffer_t* bl_command_buffer_new(int size)
{
	bl_command_buffer_t* buffer;
	
	buffer = malloc(sizeof(bl_command_buffer_t));
	
	buffer->full_size = size;
	buffer->size = 0;
	buffer->begin = 1;
	buffer->end = 0;
	
	buffer->data = malloc(sizeof(bl_command_t)*size);
	
	return buffer;
}

void bl_command_buffer_delete(bl_command_buffer_t* buffer)
{
	free(buffer->data);
	free(buffer);
}

void bl_command_buffer_queue(bl_command_buffer_t* buffer, bl_command_t cmd)
{
	if (buffer->size<buffer->full_size) {
	
		buffer->size++;
		buffer->end=(buffer->end+1)%buffer->full_size;
		buffer->data[buffer->end]=cmd;
	}
}

bl_command_t bl_command_buffer_dequeue(bl_command_buffer_t* buffer)
{
	bl_command_t cmd;
	cmd.type=BL_CMD_NONE;
	
	if (buffer->size>0) {
	
		buffer->size--;
		cmd=buffer->data[buffer->begin];
		buffer->begin=(buffer->begin+1)%buffer->full_size;
	}
	
	return cmd;
}
