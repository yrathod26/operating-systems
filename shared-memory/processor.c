#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(){
	FILE *filePointer = fopen("secrets.out","w"); /* Opens the file 'secrets.out'. If there is no such file, the program creates it.*/
	key_t key;
	char *memory;

	key = getuid(); /* Gets the uid of the user, since it is the same for a particular user, we use this as the key for accessing the shared memory. */
	int shmid = shmget(key, 1024, IPC_CREAT | 0666); /* Creating the shared memory with the authentication key, maximum allocated size, and access permission*/
	if(shmid<0){
		printf("Shared Memory Creation Failure.\n");
		exit(1);
	}
	memory = shmat(shmid, NULL, 0); /*Memory attachment with the shared memory*/
	if(memory==NULL || memory==(char *) -1){
		printf("Memory Attachment Failure.\n");
		exit(1);
	}
	while(1){
		if(*memory != '\0'){ /* Checks If something is written in the shared memory location*/
			char *s;
			int counter = 0;
			s = memory;
			while(*s != '\n'){
				if(*s=='0'||*s=='1'||*s=='2'||*s=='3'||*s=='4'||*s=='5'||*s=='6'||*s=='7'||*s=='8'||*s=='9'){
					counter++; /* Counts number of digits in the given string*/
				}
				s++;
			}
			filePointer = fopen("secrets.out","a"); /* Opens the file 'secrets.out' to append contents on the file*/
			fprintf(filePointer,"\nLine from User: %s\nNumber of Words: %d\n",memory,counter);
			printf("Input Line has been printed.\n");
			fclose(filePointer);
			*memory = '\0'; /* Clear the contents of the shared memory so the receiver can ask for input to the user*/
		}
		sleep(0.5);
	}
	if (shmdt(memory) == -1) { /*Memory detachment with the shared memory*/
		perror("Memory Detachment Failure.\n");
		exit(1);
	}
}
