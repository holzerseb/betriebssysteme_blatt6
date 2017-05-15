#include "Common.h"

int main(void)
{
	//So we request and attach some shared memory
	int shmId = RequestSharedMemory(SharedMemoryKey, 1); //true=IPC_CREATE set
	void *shmPointer = AttachSharedMemory(shmId);
	
	//copy the value 0 into it
	int zero = 0; //:^}
	memcpy(shmPointer, &zero, sizeof(int));

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
	exit(EXIT_SUCCESS);
}
