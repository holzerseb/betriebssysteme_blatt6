#define _GNU_SOURCE //ipc.h requires this...

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SharedMemoryKey 13571

const char *SemaphoreKey = "someSemaphore";

int RequestSharedMemory(key_t key, int shouldCreate);
void* AttachSharedMemory(int shmid);
int CreateSemaphore(int semid);
sem_t *GetSemaphore(int semid);

#endif
