#include "Common.h"

int main(void)
{
	//So we request and attach some shared memory
	int shm_id = RequestSharedMemory(SharedMemoryKey, 1); //true=CREATE
	void *shmPointer = AttachSharedMemory(shm_id);
	sem_t *semid = GetSemaphore(SemaphoreKey);
	
	//copy the value 0 into it
	int zero = 0; //:^}
	//we wait until we are allowed to access the shared-memory
	if(sem_wait(semid) < 0)
	{
		perror("sem_wait failed");
		exit(EXIT_FAILURE);
	}
	//once we got the shared-memory, we write
	memcpy(shmPointer, &zero, sizeof(int));
	//then we release the ressource
	if(sem_post(semid) < 0)
	{
		perror("sem_post failed");
		exit(EXIT_FAILURE);
	}
	//and since we don't need the semaphore anymore: close it
	if(sem_close(semid) < 0)
	{
		perror("sem_close failed");
		exit(EXIT_FAILURE);
	}

	//then we read a value from a fifo
	//theirfor we need some variables
	FILE *fp_fifo;
	int value; //it was stated to read an integer
		//I presume it's safe to not catch any errors here

	//create the fifo
	if(access("RESULT_FIFO", F_OK) == -1 && mkfifo("RESULT_FIFO", 0666) == -1)
	{
		perror("couldn't create the fifo");
		exit(EXIT_FAILURE);
	}
	
	//then we connect to it
	if((fp_fifo = fopen("RESULT_FIFO", "r")) == NULL)
	{
		perror("couldn't open the fifo");
		exit(EXIT_FAILURE);
	}

	//again, I rely on the fact it's definitly an integer here...
	int ffs = 0;
	while((ffs = fscanf(fp_fifo, "%d", &value)) < 1);
	fclose(fp_fifo);

	//then we ought to print it
	fprintf(stdout, "The value coming from the FIFO is: %d\n", value);
	
	//close all that unnecessary stuff
	if(sem_unlink(SemaphoreKey) < 0)
	{
		perror("sem_unlink failed");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
