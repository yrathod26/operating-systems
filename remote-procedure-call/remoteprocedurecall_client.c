#include "remoteprocedurecall.h"
#include<stdio.h>
#include<string.h>
#include <rpc/rpc.h>

void main(int numberOfArg, char *arg[])
{
 	CLIENT * client;
	char *textInput = malloc(sizeof(char)*BUFSIZ);
	//Checking if the server address is provided in the arguments, If not we have to throw a error.
	if (numberOfArg != 2){
		printf("Invalid input. Please enter correct server address. For eg: ./remoteprocedurecall_client localhost \n");
		exit(1);
    	}
	char * server = arg[1];
	while(1) {
		printf("To disconnect type 'EXIT'\n Please enter your line of text with Secret Code: "); 
		memset(textInput, 0, sizeof textInput );
		fgets(textInput, BUFSIZ, stdin); //Input the string with Secret Code C00L
			char *check = strstr (textInput,"EXIT");
			if (strstr (textInput,"EXIT")!=NULL) { //If the string has 'EXIT', close the connection and end process.
				printf("Closing Server RPC Connection because user entered 'EXIT'\n");
				exit(1);
			}
			check = strstr (textInput,"C00L");//Only if the input contains the secret code, then send it across to the server
			if (check!=NULL) {
				client = clnt_create(server,REMOTEPROCEDURECALL,REMOTEPROCEDURECALLVERSION,"tcp"); //Create a RPC call to the server stub with the server details.
				if(client == NULL){ // If RPC call fails, then print error and exit
					printf("Server not started. Couldn't connect to server!");
					exit(1);
				}
				int *result = myremoteprocedurecall_1(&textInput,client); //Save the result in the result variable
			}
		}

}
