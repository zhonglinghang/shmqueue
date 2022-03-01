#include "shmtx.h"
#include <stdio.h>

intptr_t shmtx_create(ShmMutex *mtx, const char* name) 
{
    char shmtx_file_name[100];
    strcpy(shmtx_file_name, "./Mutex_");
    if(strlen(name) > 90) {
        memcpy(&shmtx_file_name[8], name, 90);
        shmtx_file_name[98] = '\0';
    } else {
        strcat(shmtx_file_name, name);
    }
    if (mtx->name) {
        if(strcmp(shmtx_file_name, &mtx->name) == 0) {
            strcpy(&mtx->name, shmtx_file_name);
            return 0;
        } 
        shmtx_destroy(mtx);
    }
    mtx->fd = open(shmtx_file_name, O_RDWR|O_CREAT, 0666);
    printf("open errno: %d\n", errno);
    if(mtx->fd == -1) {
        return -1;
    }
    if(unlink((const char*)shmtx_file_name) == -1) {
        //exception handle
    }
    strcpy(&mtx->name, shmtx_file_name);
    return 0;    
}

void shmtx_destroy(ShmMutex *mtx) 
{
    if(close(mtx->fd) == -1) {
    }
}

int lock_fd(int fd) 
{
    struct flock fl;
    memset(&fl, 0, sizeof(struct flock));
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    int err = fcntl(fd, F_SETLKW, &fl);
    printf("lock_fd errno: %d\n", errno);
    printf("lock_fd, err: %d\n", err);
    return err;
}

int unlock_fd(int fd) 
{
    struct flock fl;
    memset(&fl, 0, sizeof(struct flock));
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    int err = fcntl(fd, F_SETLKW, &fl);
    printf("unlock_fd errno: %d\n", errno);
    printf("unlock_fd, err: %d\n", err);
    return err;
}

int shmtx_lock(ShmMutex *mtx)
{
    int err;
    err = lock_fd(mtx->fd);
    printf("shmtx_lock, err: %d\n", err);
    return err;
}

int shmtx_unlock(ShmMutex *mtx) 
{
    int err;
    err = unlock_fd(mtx->fd);
    printf("shmtx_unlock, err: %d\n", err);
    return err;
}

uintptr_t shmtx_force_unlock(ShmMutex *mtx, pid_t pid) 
{
    return 0;
}