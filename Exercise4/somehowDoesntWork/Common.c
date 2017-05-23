#include "Common.h"

//This method allocates some shared-memory-segment
int RequestSharedMemory(key_t key, int shouldCreate)
{
	int shmid;

	//Lets request some shared memory
	int flags;
	if(shouldCreate)
	{
		flags = O_RDWR | O_CREAT;
		//when we create the shared memory, we want a semaphore to be created too
		CreateSemaphore(SemaphoreKey);
	}
	else
		flags = O_RDWR;

	if((shmid = shm_open(key, flags, 0644)) < 0)
	{
		//100 is the size, IPC_CREAT lets us create a segment
		//and 0644 is the permission
		perror("shm_open failed");
		exit(EXIT_FAILURE);
	}

	return shmid;
}

//This will connect our segment to the address
void* AttachSharedMemory(int shmid)
{
	void *shmObjPtr;

	//Attaches the given shmid to the a new (suitable) address
	if((shmObjPtr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, (off_t)0)) == NULL)
						//null = find suitable address
	{
		perror("mmap failed");
		exit(EXIT_FAILURE);
	}

	return shmObjPtr;
}

sem_t *CreateSemaphore(const char *key)
{
	//create
	sem_t *semid = sem_open(key, O_CREAT, 0644, 1);
	
	return semid;
}

sem_t *GetSemaphore(const char *key)
{
	sem_t *semid = sem_open(key, 0, 0644, 1);
	return semid;
}
