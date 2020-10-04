/*
    blaster

    Copyright (C) 2020  Enrique Medina Gremaldos <quiqueiii@gmail.com>

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

#include <blaster/worker.h>

#include <stdlib.h>

void* bl_worker_draw(void* arg);
void* bl_worker_update(void* arg);

bl_worker_t* bl_worker_new(bl_raster_t* raster,uint8_t type,uint32_t flags)
{
    bl_worker_t* worker=NULL;
    
    worker = malloc(sizeof(bl_worker_t));
    worker->raster=raster;
    worker->type=type;
    
    if (type==BL_WORKER_DRAW) {
        pthread_create(&worker->thread,NULL,bl_worker_draw,worker);
    }
    else {
        pthread_create(&worker->thread,NULL,bl_worker_update,worker);
    }
    
    return worker;
}

void bl_worker_delete(bl_worker_t* worker)
{
    free(worker);
}

void bl_worker_stop(bl_worker_t* worker)
{
    pthread_cancel(worker->thread);
}

void* bl_worker_draw(void* arg)
{
    printf("draw worker ready\n");
    
    bl_worker_t* worker = (bl_worker_t*)arg;
    
    while (1) {
        bl_command_t* cmd;
        
        cmd=bl_queue_pop(worker->raster->queue_draw_commands);
        
        if (cmd==NULL) {
            break;
        }
        
        switch (cmd->type) {
            
        }
    }
    
    printf("draw worker done\n");
    
    return NULL;
}

void* bl_worker_update(void* arg)
{
    
}
