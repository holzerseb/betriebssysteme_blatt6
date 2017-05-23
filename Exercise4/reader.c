#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHM_SIZE 1024
#define MAX_BUF 1024

int main(void)
{
    int shm_fd;
    int *data;
    const char *result_fifo = "/tmp/RESULT_FIFO";
    const char *name = "shm_posix";

    // Create Shared Memory Segment
    if ((shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666)) == -1)
    {
        perror("shm_open");
        exit(1);
    }

    // Set Size of Shared Memory Segment
    ftruncate(shm_fd, SHM_SIZE);

    // Map Shared Memory Segment to Process
    if ((data = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    // Write int 0 to data pointer
    *data = 0;

    // Remove FIFO if already exists
    unlink(result_fifo);

    // Create FIFO
    if (mkfifo(result_fifo, 0666) < 0)
    {
        perror("mkfifo");
        exit(1);
    }

    // Open FIFO for Reading
    int fd = open(result_fifo, O_RDONLY);

    // Read from FIFO
    char buffer[MAX_BUF];
    read(fd, &buffer, MAX_BUF);
    printf("Message from FIFO %s\n", buffer);

    if (close(fd) == -1)
    {
        perror("close");
        exit(1);
    }

    // Remove FIFO
    unlink(result_fifo);

    return EXIT_SUCCESS;
}
