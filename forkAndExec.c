#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()  {

    char *arr[] = {NULL,NULL};
    char input[256];

    while(1) {

        printf("\nThis is the parent process with Pid = %d. Please enter your command\n",getpid());
	fgets(input, sizeof(input), stdin); //user input
        strtok(input, "\n"); 

        if((strcmp(input,"")) != 0) { //we check if there is some input given by the user.
	arr[0]=(char *)input; // We convert the (char) array to (char *) because the input of execvp needs a (char *) argument 
            pid_t pid = fork(); // Fork method is where a child process originates in parallel to the current parent process.

            if (pid == 0) { //Checking if the process is child or parent
		printf("\nWe are in child process with Pid = %d. Your command will be executed below.\n",getpid());
                if(execvp(arr[0],arr)==-1){ //We check if execvp was successfully executed and there was a result.
			printf("\nPlease enter a valid command.\n"); //If not, then the given command was not a valid command
		}
                exit(0);
            }
            else if (pid > 0) { //Checking if the process is child or parent
                wait(NULL); //waiting for the child process to complete and terminate 
            }
	    else{
		perror("fork");
		exit(0);
	    }
        }
        else
            exit(0);
    }
}
