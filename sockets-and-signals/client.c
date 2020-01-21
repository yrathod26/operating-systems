#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 9000

void main(int numberOfArg, char *arg[])
{

	char *textInput = malloc(sizeof(char)*BUFSIZ);
	int sckt = socket(AF_INET, SOCK_STREAM, 0); // Socket Creation
		struct sockaddr_in servAdd;
    memset(&servAdd, 0, sizeof servAdd );
    servAdd.sin_family = AF_INET;

	//Checking if the IP And Port are provided in the arguments, If not we have a default "127.0.0.1:9000"
	//Accordingly we have different configurations.
	if (numberOfArg == 3){
		servAdd.sin_port = htons(atoi(arg[2])); 		//Port number from command line
		servAdd.sin_addr.s_addr = inet_addr(arg[1]); 	//IP address number from command line
    }
	else{
		servAdd.sin_port = htons(PORT); 		//Default port and ip address
		servAdd.sin_addr.s_addr = inet_addr("127.0.0.1"); 		
	}
    
	//Establishing connection to the server with the socket
    if(connect(sckt, (struct sockaddr*)&servAdd, sizeof servAdd ) < 0)
    {
        printf("Server Connection Failed!"); //If connection establishment fails
        exit(1);
    }
	else{
	printf("Connected to Server, to disconnect type 'EXIT'\n");	
	}
	
	while(1) {
 	
		printf("\nPlease enter your line of text with Secret Code: "); //Input the string with Secret Code C00L
		memset(textInput, 0, sizeof textInput );
		fgets(textInput, BUFSIZ, stdin);
			char *check = strstr (textInput,"EXIT");
			if (strstr (textInput,"EXIT")!=NULL) { //If the string has 'EXIT', close the connection and end process.
				printf("Closing Server Connection because user entered 'EXIT'\n");
				if (send(sckt, textInput, strlen(textInput), 0) < 0) //Take the input of 'EXIT' and sent it to the server across the socket connection and exit
					perror("Exit Error:"); //If sending the input failed
				exit(1);
			}
			check = strstr (textInput,"C00L");//Only if the input contains the secret code, then send it across to the server
			if (check!=NULL) {
			
				if (send(sckt, textInput, strlen(textInput), 0) < 0)
					perror("Sending Error:");
			}
		}
	close(sckt); //Closing the socket if Exit is input
}
