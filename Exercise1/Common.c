#include "Common.h"

//This method allocates some shared-memory-segment
int RequestSharedMemory(key_t key, int shouldCreate)
{
	int shmid;

	//Lets request some shared memory
	int flags;
	if(shouldCreate)
		flags = IPC_CREAT | 0666;
	else
		flags = 0666;

	if((shmid = shmget(key, sizeof(int), flags)) < 0)
	{
		//100 is the size, IPC_CREAT lets us create a segment
		//and 0666 is the permission
		perror("shmget failed");
		exit(EXIT_FAILURE);
	}

	return shmid;
}

//This will connect our segment to the address
void* AttachSharedMemory(int shmid)
{
	void *shmPointer;

	//Attaches the given shmid to the a new (suitable) address
	if((shmPointer = shmat(shmid, NULL, 0)) == NULL)
	{
		perror("shmat failed");
		exit(EXIT_FAILURE);
	}

	return shmPointer;
}
