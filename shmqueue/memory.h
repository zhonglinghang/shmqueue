#ifndef INCLUDE_SHMQUEUE_MEMORY_H_
#define INCLUDE_SHMQUEUE_MEMORY_H_
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>
#include "shmtx.h"
#include <stdint.h>
#include <unistd.h>

#define SHMALIGN(s, a) (((s - 1) | (a  - 1)) + 1)
#define AV_SIZE_DATA_ 8
#define MAX_NAME_LEN 256
static const size_t QUEUE_SIZE = 16;
static const int AV_SIZE_DATA = 2048 * 1024 * 4;
static size_t buffer_size = 4;
static size_t GAP = 1024; 

typedef struct ShmMemBlock {
    void *ptr;
    size_t size;
} ShmMemBlock;

typedef struct Element {
    bool empty;
    size_t size;
    uint8_t data[AV_SIZE_DATA];
} Element;

typedef struct QueueElem {
    char callback[MAX_NAME_LEN];
    Element msg;
} QueueElem;

typedef struct SharedQueue {
    uint32_t cursor;
    uint32_t counter;
    QueueElem elements[QUEUE_SIZE];
} SharedQueue;

typedef struct Memory {
    SharedQueue *shared_queue;
} Memory;

void* create_memory_segment(const char *name, size_t size, int *new_segment, size_t alignment);

ShmMemBlock* shm_memblock_create(void *ptr, size_t size);

void shm_memblock_release(ShmMemBlock* ShmMemBlock);

Memory* shm_memory_create(const char* name);

void shm_shared_queue_init(Memory* mem);

size_t shm_shared_queue_size(Memory* mem);

void shm_memory_release(Memory* mem);

#endif // INCLUDE_SHMQUEUE_MEMORY_H_