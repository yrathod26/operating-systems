#include "remoteprocedurecall.h"
#include<stdio.h>
#include<string.h>
#include <rpc/rpc.h>
#include <signal.h>

int numberOfLines=0,numberOfDigits=0;
FILE * filePointer;

/*Handles the Ctrl-C interrupt and displays the summary*/
void ctrCHandler(int s){
		printf("\nTotal Number of Lines Recieved from client: %d \n",numberOfLines);
		printf("\nTotal Number of Digits Recieved from client: %d \n ",numberOfDigits);

}

/* Remote Call (RPC) to the server stub from the client will be made through this method */
int *myremoteprocedurecall_1_svc(char **inputString, struct svc_req *rqstp)
{
	signal (SIGINT,ctrCHandler); //This handles the Ctrl-C interrupt
	int currNumDigits=0;
	static int result;
	int length = strlen(*inputString);
	char *check;
	check = strstr (*inputString,"EXIT"); //If the Input String is 'EXIT', then the connection is to be closed
	if (check!=NULL) {
		printf("Closing Connection because user entered 'EXIT'\n");
	exit(1);}
	check = strstr (*inputString,"C00L"); //Checking if the input String has the secret code 'C00L'
	if (check!=NULL) {
	numberOfLines++; //If it has the secret code. then increase the numberOfLines Count by 1

		filePointer = fopen("secrets.out","a"); //Opening the secret.out file to print the line and digits count
		int k = 0;
		char *s = *inputString;
		while (*s != '\n') { //Counting the number of digits in the input
		    if (*s=='0'||*s=='1'||*s=='2'||*s=='3'||*s=='4'||*s=='5'||*s=='6'||*s=='7'||*s=='8'||*s=='9')
		       	k++;
		  s++;
		}
		currNumDigits = k;
		numberOfDigits = numberOfDigits + currNumDigits; //Increasing the global count for number of digits
		fprintf(filePointer,"\nLine from User: %s\nNumber of Digits: %d\n",*inputString,currNumDigits); //Print count in the file
		printf("\nLine from User: %s\nNumber of Digits: %d\n",*inputString,currNumDigits); //Print count in the file
		fclose(filePointer);
	}

	return &result; // Return the result to the client
}
