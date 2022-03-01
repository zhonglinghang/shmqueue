#ifndef INCLUDE_SHMQUEUE_SUBSCRIBER_H_
#define INCLUDE_SHMQUEUE_SUBSCRIBER_H_
#include <memory.h>
#include <string.h>
#include "queue.h"

typedef struct Subscriber {
    char* queue_name;
    ShmQueue* queue;
} Subscriber;

Subscriber *shm_subscriber_create(const char* queue_name);

ShmMemBlock *shm_subscriber_consumer(Subscriber *subscriber);

void shm_subscriber_release(Subscriber *subscriber);

#endif //INCLUDE_SHMQUEUE_SUBSCRIBER_H_