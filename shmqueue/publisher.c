#include "publisher.h"

Publisher *shm_publisher_create(const char *queue_name)
{
    Publisher *publisher = malloc(sizeof(Publisher));
    publisher->queue_name = queue_name;
    publisher->queue = shm_queue_create(queue_name);
    return publisher;
}

int shm_publisher_publish(Publisher *publisher, void *data, size_t size) {
    ShmMemBlock *memblock = shm_memblock_create(data, size);
    shm_queue_write(publisher->queue, memblock);
    shm_memblock_release(memblock);
    return 0;
}

void shm_publisher_release(Publisher *publisher) {
    if(publisher) {
        shm_queue_release(publisher->queue);
        free(publisher);
    }
}