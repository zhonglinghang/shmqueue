#ifndef INCLUDE_SHMQUEUE_PUBLISHER_H_
#define INCLUDE_SHMQUEUE_PUBLISHER_H_

#include <memory.h>
#include <string.h>

#include "queue.h"

typedef struct Publisher {
    char* queue_name;
    ShmQueue *queue;
} Publisher;

Publisher *shm_publisher_create(const char *queue_name);

int shm_publisher_publish(Publisher *publisher, void *data, size_t size);

void shm_publisher_release(Publisher *publisher);
#endif