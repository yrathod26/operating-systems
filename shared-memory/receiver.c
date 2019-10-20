#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(){

	char input[1024];
	key_t key;
	char *memory;

	key = getuid(); /* Gets the uid of the user, since it is the same for a particular user, we use this as the key for accessing the shared memory. */
	int shmid = shmget(key, 1024, IPC_CREAT | 0666);/* Creating the shared memory with the authentication key, maximum allocated size and access permission*/
	if(shmid<0){
		printf("Shared Memory Creation Failure.\n");
		exit(1);
	}
	memory = shmat(shmid, NULL, 0); /*Memory attachment with the shared memory*/
	if(memory==NULL){
		printf("Memory Attachment Failure.\n");
		exit(1);
	}
	while(1){
		printf("\nPlease enter your line of text.\n");
		fgets(input, 1024, stdin); /*Take input string from user*/
		if(strstr (input,"C00L"))
			memcpy(memory,input,1024); /*Copy the string to the shared memory location*/
			while(*memory!='\0') { /*Wait until the processor has read the memory location and processed the string in it*/
				sleep(1);
			}
	}
}
