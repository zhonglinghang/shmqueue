#ifndef INCLUDE_SHMQUEUE_QUEUE_H_
#define INCLUDE_SHMQUEUE_QUEUE_H_

#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "shmtx.h"

typedef struct ShmQueue {
    char name[MAX_NAME_LEN];
    ShmMutex* mtx;
    Memory* memory;
} ShmQueue;

ShmQueue* shm_queue_create(const char* queue_name);

int shm_queue_write(ShmQueue* queue, ShmMemBlock* memblock);

ShmMemBlock* shm_queue_read(ShmQueue* queue); 

size_t shm_queue_size(ShmQueue* queue);

void shm_queue_release(ShmQueue* queue);

inline __attribute__((always_inline)) void shm_queue_counter_inc(ShmQueue* queue){
    queue->memory->shared_queue->counter++;
}

inline __attribute__((always_inline)) uint32_t shm_queue_counter(ShmQueue* queue) {
    return queue->memory->shared_queue->counter;
} 

inline __attribute__((always_inline)) void shm_queue_cursor_inc(ShmQueue* queue){
    queue->memory->shared_queue->cursor++;
}

inline __attribute__((always_inline)) uint32_t shm_queue_cursor(ShmQueue* queue) {
    return queue->memory->shared_queue->cursor;
}

inline __attribute__((always_inline)) void shm_queue_cursor_reset(ShmQueue* queue) {
    queue->memory->shared_queue->cursor = shm_queue_counter(queue);
}

#endif //INCLUDE_SHMQUEUE_QUEUE_H_