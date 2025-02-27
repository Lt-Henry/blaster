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

#ifndef BLASTER_WORKER
#define BLASTER_WORKER

#ifdef __cplusplus
extern "C" {
#endif

#include "constants.h"

#include <stdint.h>
#include <pthread.h>

#define BL_WORKER_DRAW 0
#define BL_WORKER_UPDATE 1

typedef struct {
    uint8_t type;
    pthread_t thread;
    
    void* args;
    
    struct {
        uint64_t wait;
        uint64_t work;
        uint64_t start;
        uint64_t last;
    } time;

    struct {
        uint64_t chunks;
    } update;
    
} bl_worker_t;

bl_worker_t* bl_worker_new(uint8_t type,uint32_t flags);

void bl_worker_start(bl_worker_t* worker,void *(*func) (void *),void* args);

void bl_worker_delete(bl_worker_t* worker);

void bl_worker_stop(bl_worker_t* worker);

#ifdef __cplusplus
}
#endif

#endif
