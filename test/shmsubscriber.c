#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include "publisher.h"
#include "subscriber.h"
#include "memory.h"

int main(int argc, char *argv[]) 
{
    struct dirent *entry;
    const char* queue = "multiple_queue";
    if(argc != 2){
        printf("usage: /shmsubscriber [message]");
        return 0;
    }
    int num = atoi(argv[1]);
    Subscriber *sub = shm_subscriber_create(queue);

    int msg = 1;
    int cnt = 0;
    for(int i = 0; i < num; i++){
        ShmMemBlock *memBlock = shm_subscriber_consumer(sub);
        if (memBlock) {
            int answer = *((int *)(memBlock->ptr));
            printf("get message: i %d, answer: %d\n", i, answer);
            shm_memblock_release(memBlock);
        } else {
            printf("no message \n");
        }
        usleep(100000);
    }

}