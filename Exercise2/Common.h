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

#define SharedMemoryKey 13571
#define SemaphoreKey 13572

int RequestSharedMemory(key_t key, int shouldCreate);
void* AttachSharedMemory(int shmid);
int CreateSemaphore(int semid);
int GetSemaphore(int semid);

#endif
