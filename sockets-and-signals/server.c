#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 9000

int numberOfLines=0,numberOfDigits=0; // Storing Number of digits and line count for global sum

//This function is called when Ctrl+C is pressed. The function displays the lines and number of digits.
void ctrCHandler(int s){
		printf("\nTotal Number of Lines Recieved from client: %d, Total Number of Digits Recieved from client: %d \n",numberOfLines,numberOfDigits);

}

void main()
{
	signal (SIGINT,ctrCHandler); // Ctrl+C Handler is called
	int sckt, tempInp,currNumDigits;
	struct sockaddr_in servAdd,clientAdd;
	char s[200], inputString[BUFSIZ];
	
	FILE * filePointer = fopen("secrets.out","w");

	sckt= socket(AF_INET,SOCK_STREAM,0); //Socket initialization

	memset(&servAdd, 0, sizeof(servAdd));
	servAdd.sin_family = AF_INET; //Server Connection Configurations
	servAdd.sin_port = htons(PORT); //Setting the default Port number '9000'
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY); //Setting the default local IP address '127.0.0.1'

	bind(sckt,(struct sockaddr*)&servAdd,sizeof(servAdd)); //Binding with the server at the given IP and Port

	listen(sckt, 1); //Checks for incoming connection from the client and waits if not found
	printf("Waiting for client connection....\n");
	socklen_t size;
	size = sizeof(clientAdd);
	tempInp = accept(sckt, (struct sockaddr *)&clientAdd, &size);
	printf("Established connection from the client at %s \n", inet_ntop(AF_INET, &clientAdd.sin_addr, s, sizeof (s)));

	char *check;
	while(1) 
		{
		memset(inputString, 0, sizeof inputString ); //Reset InputString
		recv(tempInp, inputString, BUFSIZ-1, 0); //Recieve inputString from the socket connection from the client
			check = strstr (inputString,"EXIT"); //If the Input String is 'EXIT', then the connection is to be closed
			if (check!=NULL) {
				printf("Closing Connection because user entered 'EXIT'\n");
			exit(1);}

		check = strstr (inputString,"C00L"); //Checking if the input String has the secret code 'C00L'
			if (check!=NULL) {
			numberOfLines++; //If it has the secret code. then increase the numberOfLines Count by 1
		printf("Recieved from Client: %s \n",inputString); //Display the inputString from Client
		
				filePointer = fopen("secrets.out","a"); //Opening the secret.out file to print the line and digits count
				int k = 0;
				char *s = inputString;
				while (*s != '\n') { //Counting the number of digits in the input
				    if (*s=='0'||*s=='1'||*s=='2'||*s=='3'||*s=='4'||*s=='5'||*s=='6'||*s=='7'||*s=='8'||*s=='9')
				       	k++;
				  s++;
				}
				currNumDigits = k;
				numberOfDigits = numberOfDigits + currNumDigits; //Increasing the global count for number of digits
				fprintf(filePointer,"\nLine from User: %s\nNumber of Digits: %d\n",inputString,currNumDigits); //Print count in the file
				fclose(filePointer);
			}	
			
		close(sckt); // If 'EXIT' is input, then close the socket and exit the while loop
	}
}
