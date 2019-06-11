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

bl_queue_t* bl_queue_new(int size)
{
    bl_queue_t* queue=NULL;
    
    queue=malloc(sizeof(bl_queue_t));
    
    queue->data=malloc(size*sizeof(void*));
    queue->capacity = size;
    queue->size = 0;
    queue->begin = 1;
    queue->end = 0;
    
    atomic_flag_clear(&queue->lock);
    
    return queue;
}

void bl_queue_delete(bl_queue_t* queue)
{
    free(queue->data);
    free(queue);
}

void bl_queue_push(bl_queue_t* queue,void* value)
{
    loop:
    
    while (atomic_flag_test_and_set(&queue->lock)) {
    }
    
    if (queue->size>=queue->capacity) {
        atomic_flag_clear(&queue->lock);
        goto loop;
    }
    
    queue->size++;
    queue->end=(queue->end+1)%queue->capacity;
    queue->data[queue->end]=value;
    
    atomic_flag_clear(&queue->lock);
    
}

void* bl_queue_pop(bl_queue_t* queue)
{
    void* value;
    
    loop:
    
    while (atomic_flag_test_and_set(&queue->lock)) {
    }
    
    if (queue->size<=0) {
        atomic_flag_clear(&queue->lock);
        goto loop;
    }
    
    queue->size--;
    assert(queue->size>=0);
    value=queue->data[queue->begin];
    queue->begin=(queue->begin+1)%queue->capacity;
    
    atomic_flag_clear(&queue->lock);
    
    return value;
}

void bl_queue_clear(bl_queue_t* queue)
{
    while (atomic_flag_test_and_set(&queue->lock)) {
    }
    
    queue->size=0;
    queue->begin=1;
    queue->end=0;
    
    atomic_flag_clear(&queue->lock);
    
}

int bl_queue_is_empty(bl_queue_t* queue)
{
    return (queue->size<=0);
}

int bl_queue_is_full(bl_queue_t* queue)
{
    return (queue->size==queue->capacity);
}
