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

bl_worker_t* bl_worker_new(uint8_t type,uint32_t flags)
{
    bl_worker_t* worker=NULL;
    
    worker = malloc(sizeof(bl_worker_t));
    worker->type=type;
    worker->time.wait=0;
    worker->time.work=0;
    
    //pthread_create(&worker->thread,NULL,bl_worker_draw,worker);
    return worker;
}

void bl_worker_start(bl_worker_t* worker,void *(*func) (void *),void* args)
{
    worker->args=args;
    pthread_create(&worker->thread,NULL,func,worker);
}

void bl_worker_delete(bl_worker_t* worker)
{
    free(worker);
}

void bl_worker_stop(bl_worker_t* worker)
{
    pthread_cancel(worker->thread);
}

