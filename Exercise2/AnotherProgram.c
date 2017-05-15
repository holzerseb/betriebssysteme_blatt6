#include "Common.h"
#include <wait.h>

//:)
int main(void)
{
	//We request the createdMemory here
	int shmId = RequestSharedMemory(SharedMemoryKey, 0); //false==use existing shm
	void *shmPointer = AttachSharedMemory(shmId);
	int semid = GetSemaphore(SemaphoreKey);
	
	//because fork inherits all memory-segments, we can now simply create processes
	int pid;
	for(int i=100; i > 0; i--)
	{
		if((pid = fork()) < 0)
		{ //I ain't gonna handle the errors properly here
			perror("error creating a process");
			exit(EXIT_FAILURE);
		}
		else if(pid == 0)
		{ //this mah child
			for(int l=100; l > 0; l--)
			{
				//we wait until we are allowed to access the shared-memory
				struct sembuf sops[1];
				sops[0].sem_num = 0; //we want to access semaphore 0
				sops[0].sem_op = -1; //when semaphore == 1, we get access, semaphore will be decremented then
				sops[0].sem_flg = 0;
				if(semop(semid, sops, 1) < 0)
				{
					perror("semop failed");
					exit(EXIT_FAILURE);
				}
				//once we got the shared-memory, we write
				(*(int*)shmPointer)++;
				//then we release the ressource
				sops[0].sem_op = +1;
				if(semop(semid, sops, 1) < 0)
				{
					perror("semop failed");
					exit(EXIT_FAILURE);
				}

			}

			//exit removes the attached shared memory as well
			exit(EXIT_SUCCESS);
		}
	}
	
	//the parent then waits
	pid_t wpid;
	int status = 0;
	while((wpid = wait(&status)) > 0); //waits for all children
	
	//print the value to the stdout
	fprintf(stdout, "The Value in the Shared-Memory-Segment is: %d\n", *(int*)shmPointer);
	
	//and into the pipe
	FILE *fp_fifo;
	
	//then we connect to it
	if((fp_fifo = fopen("RESULT_FIFO", "w")) == NULL)
	{
		perror("couldn't open the fifo");
		exit(EXIT_FAILURE);
	}
	fprintf(fp_fifo, "%d", *(int*)shmPointer);

	fclose(fp_fifo);
	shmdt(shmPointer);
	exit(EXIT_SUCCESS);
}
