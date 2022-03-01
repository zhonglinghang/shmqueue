#include "subscriber.h"

Subscriber *shm_subscriber_create(const char *queue_name) {
    Subscriber *subscriber = malloc(sizeof(Subscriber));
    subscriber->queue_name = queue_name;
    subscriber->queue = shm_queue_create(queue_name);
    return subscriber;
}

ShmMemBlock *shm_subscriber_consumer(Subscriber* subscriber) {
    if(shm_queue_counter(subscriber->queue) <= shm_queue_cursor(subscriber->queue)) {
        return NULL;
    }

    if(shm_queue_counter(subscriber->queue) - shm_queue_cursor(subscriber->queue) > shm_queue_size(subscriber->queue)) {
        shm_queue_cursor_reset(subscriber->queue);
    }

    ShmMemBlock *memblock = shm_queue_read(subscriber->queue);
    return memblock;
}

void shm_subscriber_release(Subscriber *subscriber) {
    if(subscriber) {
        shm_queue_release(subscriber->queue);
        free(subscriber);
    }
}