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

#ifndef BLASTER_MATRIX
#define BLASTER_MATRIX

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

/*! async queue */
typedef struct {

    void** data;
    int capacity;
    int begin;
    int end;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t full;
    pthread_cond_t empty;
} bl_queue_t;

bl_queue_t* bl_queue_new(int size);

void bl_queue_delete(bl_queue_t* queue);

void bl_queue_push(bl_queue_t* queue,void* value);

void* bl_queue_pop(bl_queue_t* queue);

void bl_queue_clear(bl_queue_t* queue);

#endif
