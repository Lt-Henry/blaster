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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

bl_queue_t* bl_queue_new(int size)
{
    bl_queue_t* queue=NULL;
    
    queue=malloc(sizeof(bl_queue_t));
    
    queue->data=malloc(size*sizeof(void*));
    queue->capacity = size;
    queue->size = 0;
    queue->begin = 1;
    queue->end = 0;
    
    pthread_mutex_init(&queue->mutex,NULL);
    pthread_cond_init(&queue->full,NULL);
    pthread_cond_init(&queue->empty,NULL);
    
    return queue;
}

void bl_queue_delete(bl_queue_t* queue)
{
    pthread_cond_destroy(&queue->full);
    pthread_cond_destroy(&queue->empty);
    pthread_mutex_destroy(&queue->mutex);
    
    free(queue->data);
    free(queue);
}

void bl_queue_push(bl_queue_t* queue,void* value)
{

    pthread_mutex_lock(&queue->mutex);
    
    while (queue->size >= queue->capacity) {
        pthread_cond_wait(&queue->full,&queue->mutex);
    }
    
    queue->size++;
    queue->end=(queue->end+1)%queue->capacity;
    queue->data[queue->end]=value;
    
    pthread_cond_signal(&queue->empty);
    pthread_mutex_unlock(&queue->mutex);
    
}

void* bl_queue_pop(bl_queue_t* queue)
{
    void* value;
    
    pthread_mutex_lock(&queue->mutex);
    
    while (queue->size==0) {
        pthread_cond_wait(&queue->empty,&queue->mutex);
    }
    
    queue->size--;
    assert(queue->size>=0);
    value=queue->data[queue->begin];
    queue->begin=(queue->begin+1)%queue->capacity;
    
    pthread_cond_signal(&queue->full);
    
    pthread_mutex_unlock(&queue->mutex);
    
    return value;
}

void bl_queue_clear(bl_queue_t* queue)
{
    pthread_mutex_lock(&queue->mutex);
    
    queue->size=0;
    queue->begin=1;
    queue->end=0;
    
    pthread_mutex_unlock(&queue->mutex);
    
}

int bl_queue_is_empty(bl_queue_t* queue)
{
    pthread_mutex_lock(&queue->mutex);
    int ret = (queue->size<=0);
    pthread_mutex_unlock(&queue->mutex);
    
    return ret;
}

int bl_queue_is_full(bl_queue_t* queue)
{
    
    pthread_mutex_lock(&queue->mutex);
    int ret = (queue->size==queue->capacity);
    pthread_mutex_unlock(&queue->mutex);
    
    return ret;
}
