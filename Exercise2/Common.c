#include "Common.h"

//This method allocates some shared-memory-segment
int RequestSharedMemory(key_t key, int shouldCreate)
{
	int shmid;

	//Lets request some shared memory
	int flags;
	if(shouldCreate)
	{
		flags = IPC_CREAT | 0666;
		//when we create the shared memory, we want a semaphore to be created too
		CreateSemaphore(SemaphoreKey);
	}
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

/* POSIX.1-2001 requires the caller to define this union
* what a sh*t, why would somebody do this??
* This union was defined in sys/sem.h but isn't anymore... */
union semun
{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int CreateSemaphore(key_t key)
{
	int semid;
	
	//create
	semid = semget(key, 1, IPC_CREAT | 0666); //we need only 1 semaphore with 0 flags

	//initialize the semaphore with index 0
	union semun arg;
	arg.val = 1; //only one process can access the shared memory at a time
	if(semctl(semid, 0, SETVAL, arg))
	{
		perror("semctl failed");
		exit(EXIT_FAILURE);
	}
	
	return semid;
}

int GetSemaphore(key_t key)
{
	int semid = semget(key, 0, 0666); //the first 0 is required if we open an existing semaphore
	return semid;
}
