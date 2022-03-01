#include "memory.h"

uint8_t *align_address(void *ptr, size_t alignment) 
{
    unsigned long int_ptr = (unsigned long)ptr;
    uint8_t *aligned_int_ptr = (uint8_t*)SHMALIGN(int_ptr, alignment);
    return (uint8_t*)(aligned_int_ptr);
}

void* create_memory_segment(const char *name, size_t size, int *new_segment, size_t alignment) 
{
    printf("Creating memory segment, name: %c, size: %d\n", name, size);
    int fd;
    while(1) {
        *new_segment = 1;
        fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0644);
        if (fd >= 0) {
            fchmod(fd, 0644);
        } else if (errno = EEXIST) {
            fd = shm_open(name, O_RDWR, 0644);
            if(fd < 0 && errno == ENOENT) {
                continue;
            }
            *new_segment = 0;    
        } else {
            return NULL;
        }
        break;
    }

    if (*new_segment) {
        int result = ftruncate(fd, size + alignment);
        if (result == EINVAL) {
            return NULL;
        }
    }
    auto *ptr = mmap(NULL, size + alignment, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    return align_address(ptr, alignment);
}

ShmMemBlock* shm_memblock_create(void* ptr, size_t size) 
{
    ShmMemBlock* mblock = malloc(sizeof(ShmMemBlock));
    mblock->ptr = malloc(size);
    memcpy(mblock->ptr, ptr, size);
    mblock->size = size;
    return mblock;
} 

void shm_memblock_release(ShmMemBlock* memblock) 
{
    if(memblock) {
        if(memblock->ptr) {
            free(memblock->ptr);
        }
        free(memblock);
    }
}

Memory* shm_memory_create(const char* name) 
{
    size_t shared_queue_size = sizeof(SharedQueue);
    size_t total_size = shared_queue_size + GAP;
    int new_segment = 0;
    char shm_name[100];
    strcpy(shm_name, "/SHM_");
    strcat(shm_name, name);
    uint8_t *base_address = create_memory_segment(shm_name, total_size, &new_segment, 32);
    if (base_address == NULL) {
        printf("Could not create/open shared memory segment.\n");
        return NULL;
    }    
    Memory* mem = malloc(sizeof(Memory));
    uint8_t *shared_queue_address = base_address;
    if(new_segment) {
        mem->shared_queue = (SharedQueue*)(shared_queue_address);
        shm_shared_queue_init(mem);
    } else {
        mem->shared_queue = (SharedQueue*) (shared_queue_address);
        usleep(10000);
    }
    return mem;
}

void shm_shared_queue_init(Memory* mem) 
{
    mem->shared_queue->counter = 0;
    mem->shared_queue->cursor = 0;
}

size_t shm_shared_queue_size(Memory* mem) 
{
    return QUEUE_SIZE;
}

void shm_memory_release(Memory* mem) 
{
    if(mem) {
        free(mem);
    }
}