#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SHM_SIZE 1024
#define MAX_BUF 1024

int main(void)
{
    int shm_fd;
    int *data;
    int size = sizeof(int*);
    const char *result_fifo = "/tmp/RESULT_FIFO";
    const char *name = "shm_posix";

    // Create Shared Memory Segment
    if ((shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666)) == -1)
    {
        perror("shm_open");
        exit(1);
    }

    // Map Shared Memory Segment to Process
    if ((data = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    // Create Semaphore and initialize with 1
    sem_t* semptr = (sem_t*)(data + size);

    if (sem_init(semptr, 1, 1) == -1)
    {
        perror("sem_init");
        exit(1);
    }

    int children_count = 100;
    int increment_count = 100;
    int val;
    pid_t pid;

    // Child Processes
    for (int i = 0; i < children_count; ++i)
    {
        if ((pid = fork()) < 0)
        {
            perror("fork");
            abort();
        }
        else if (pid == 0)
        {
            // Lock Semaphore for Writing
            sem_wait(semptr);

            for (int i = 0; i < increment_count; ++i)
            {
                val = *data;
                *data = val + 1;
            }

            // Unlock Semaphore
            sem_post(semptr);

            exit(0);
        }
    }

    // Parent Process
    // Wait for all Children to terminate
    for (int i = 0; i < children_count; ++i)
    {
        wait(NULL);
    }

    int fifo_fd;
    if ((fifo_fd = open(result_fifo, O_WRONLY)) < 0)
    {
        perror("openfifo");
        exit(1);
    }

    char buffer[MAX_BUF];

    snprintf(buffer, MAX_BUF, "%d", *data);
    write(fifo_fd, &buffer, MAX_BUF);

    // Close FIFO
    if (close(fifo_fd) == -1)
    {
        perror("fclose");
        exit(1);
    }

    // Destroy Semaphore
    sem_destroy(semptr);

    // Destroy Shared Memory
    shm_unlink(name);

    return 0;
}
