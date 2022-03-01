#include "queue.h"

ShmQueue* shm_queue_create(const char* queue_name) 
{
    ShmQueue *shm_queue = malloc(sizeof(ShmQueue));
    shm_queue->memory = shm_memory_create(queue_name);
    shm_queue->mtx = (ShmMutex*) malloc(sizeof(ShmMutex));
    shmtx_create(shm_queue->mtx, queue_name);
    return shm_queue;
}

int shm_queue_write(ShmQueue *queue, ShmMemBlock *memblock) 
{
    shmtx_lock(queue->mtx);
    if(shm_queue_counter(queue) - shm_queue_cursor(queue) >= shm_queue_size(queue)) {
        printf("Increase buffer size");
        shmtx_unlock(queue->mtx);
        return -1;
    }
    uint32_t q_pos = shm_queue_counter(queue) & (shm_queue_size(queue) - 1);
    QueueElem *elem = &(queue->memory->shared_queue->elements[q_pos]);
    {
        memcpy(&elem->msg.data, memblock->ptr, memblock->size);
        elem->msg.size = memblock->size;
        elem->msg.empty = false;
        shm_queue_counter_inc(queue);
    }
    shmtx_unlock(queue->mtx);
    return 0;
}

ShmMemBlock* shm_queue_read(ShmQueue *queue) 
{
    shmtx_lock(queue->mtx);
    if(shm_queue_size(queue) < (shm_queue_counter(queue) - shm_queue_cursor(queue))) {
        shm_queue_cursor_reset(queue);
    }
    uint32_t q_pos = shm_queue_cursor(queue) & (shm_queue_size(queue) - 1);
    QueueElem *elem = &(queue->memory->shared_queue->elements[q_pos & (shm_queue_size(queue) - 1)]);
    if (elem->msg.empty) {
        shmtx_unlock(queue->mtx);
        return NULL;
    }
    ShmMemBlock *memblock = shm_memblock_create(&elem->msg.data, elem->msg.size);
    memblock->size = elem->msg.size;
    memcpy(memblock->ptr, &elem->msg.data, memblock->size);
    elem->msg.empty = true;
    shm_queue_cursor_inc(queue);
    shmtx_unlock(queue->mtx);
    return memblock;
}

size_t shm_queue_size(ShmQueue *queue) 
{
    return shm_shared_queue_size(queue->memory);
}

void shm_queue_release(ShmQueue *queue) 
{
    if(queue) {
        shm_memblock_release(queue->memory);
        if(queue->mtx) {
            shmtx_destroy(queue->mtx);
            free(queue->mtx);
        }
        free(queue);
    }
}