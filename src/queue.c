/*
    blaster

    Copyright (C) 2018  Enrique Medina Gremaldos <quiqueiii@gmail.com>

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

#include <blaster/queue.h>

#include <string.h>


bl_queue_t* bl_queue_new(int size,size_t bucket_size)
{
    bl_queue_t* queue=NULL;
    
    queue=malloc(sizeof(bl_queue_t));
    
    queue->data=malloc(size*bucket_size);
    queue->capacity = size;
    queue->size = 0;
    queue->begin = 1;
    queue->end = 0;
    
    pthread_mutex_init(&queue->mutex,NULL);
    
    return queue;
}

void bl_queue_delete(bl_queue_t* queue)
{
    pthread_mutex_destroy(queue->mutex);
    free(queue->data);
    free(queue);
}

int bl_queue_push(bl_queue_t* queue,void* value)
{
    int ret;
    
    pthread_mutex_lock(queue->mutex);
    
    if (queue->size < queue->capacity) {
        queue->size++;
        queue->end=(queue->end+1)%queue->capacity;
        //queue->data[queue->end]=cmd;
        ret=0;
    }
    else {
        ret=1;
    }
    
    pthread_mutex_unlock(queue->mutex);
    
    return ret;
}

int bl_queue_pop(bl_queue_t* queue,void* value)
{
    pthread_mutex_lock(queue->mutex);
    
    pthread_mutex_unlock(queue->mutex);
}

void bl_queue_clear(bl_queue_t* queue)
{
    pthread_mutex_lock(queue->mutex);
    
    queue->size=0;
    queue->begin=1;
    queue->end=0;
    
    pthread_mutex_unlock(queue->mutex);
}
