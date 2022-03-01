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
        printf("usage: /shmpulisher [message]");
        return 0;
    }
    int num = atoi(argv[1]);
    Publisher *pub = shm_publisher_create(queue);

    int msg = 1;
    int cnt = 0;
    for(int i = 0; i < num; i++){
        shm_publisher_publish(pub, (void *)&msg, sizeof(int));
        printf("send message: i %d, message: %d\n", i, msg);
        msg++;
        usleep(100000);
    }

}