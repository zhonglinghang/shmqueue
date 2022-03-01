#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include "publisher.h"
#include "subscriber.h"
#include "memory.h"

int main(int argc, char *argv[]) 
{
    struct dirent *entry;
    const char* queue = "multiple_queue_test_";

    int n_pubs = 3, n_subs = 3, n_messages = 5;

    Publisher *pubs[n_pubs];
    for(int i = 0; i < n_pubs; i++) {
        Publisher *pub = shm_publisher_create(queue);
        pubs[i] = pub;
    }

    int messages[15];
    int msg = 1;
    int cnt = 0;
    for(int i = 0; i < n_pubs; i++){
        for(int j = 0; j < n_messages; j++) {
            messages[cnt] = msg;
            shm_publisher_publish(pubs[i], (void *)&msg, sizeof(int));
            msg++;
        }
    }

    int vec_answers[3][10];
    Subscriber *subs[n_subs];
    for(int i = 0; i < n_subs; i++) {
        Subscriber *sub = shm_subscriber_create(queue);
        subs[i] = sub;
        for(int j = 0; j < n_messages; j++) {
            ShmMemBlock* mblock = shm_subscriber_consumer(sub);
            int answer = *((int *)(mblock->ptr));
            vec_answers[i][j] = answer;
            printf("answers, i: %d, j: %d, answer: %d \n", i, j, vec_answers[i][j]);
            shm_memblock_release(mblock);
        }
    }
    cnt = 0;
    for(int i = 0; i < n_pubs; i++){
        for(int j = 0; j < n_messages; j++) {
            messages[cnt] = msg;
            shm_publisher_publish(pubs[i], (void *)&msg, sizeof(int));
            msg++;
        }
    }

}