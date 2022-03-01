#ifndef INCLUDE_SHMQUEUE_SHMTX_H_
#define INCLUDE_SHMQUEUE_SHMTX_H_
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

typedef uint64_t  shm_atomic_uint_t;
typedef volatile shm_atomic_uint_t shm_atomic_t;

typedef struct ShmMutex {
    int fd;
    char name[100];
} ShmMutex;

intptr_t shmtx_create(ShmMutex *mtx, const char* name);
void shmtx_destroy(ShmMutex *mtx);
uintptr_t shmtx_trylock(ShmMutex *mtx);
int shmtx_lock(ShmMutex *mtx);
int shmtx_unlock(ShmMutex *mtx);
uintptr_t shmtx_force_unlock(ShmMutex *mtx, pid_t pid);

#endif // INCLUDE_SHMQUEUE_SHMTX_H_