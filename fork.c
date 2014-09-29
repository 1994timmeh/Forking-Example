#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Simple message sending from Child to Parent using fork() and dup2
 * Author:  Tim Hadwen
 * Modified: 29 September 2014
 * 
 * Tested under gcc and clang
 */
int main(){
	/* Create an array of Ints to hold the 2 File Descriptors for the Pipe*/
	int fd[2];

	/* Initiate the pipe */
	pipe(fd);

	if(fork()){
		//Parent Process
		/* Close the writing end of the pipe */
		close(fd[1]);

		/* Connect fd[0] to STD_IN (0 is STD_IN)  */
		dup2(fd[0], 0);
		
		/* Make some buffer variables to store the message we will receive*/
		char buffer;
		char message[20];
		int messageCount = 0;
	
		/* Receive some data from STD_IN 
			REMEMBER: This is connected to the pipe to the child process */
		while((buffer = fgetc(stdin))){
			printf("Received char %c\n", buffer);
			if(buffer == 'M'){
				printf("Starting message receive\n");
				messageCount = 0;
			} else if (buffer == 'X'){
				/* Message complete if we receive an X */
				printf("Message has completed\n");
				printf("Message from client> %s\n", message);
				exit(0);
			} else {
				/* Received a charecter to add to the message */
				message[messageCount] = buffer;
				messageCount++;
			}
		}
		
	} else {
		//Child Process
		/* Close the reading end of the fd as the child is
			only sending */
		close(fd[0]);

		/* Connect STD_OUT to the pipe (1 is STD_OUT) */
		dup2(fd[1], 1);

		/* IMPORTANT: Close the pipe as it is now connected to STD_OUT of the child
			process */
		close(fd[1]);

		/* Send some data to STD_OUT
			REMEMBER: This is connected to the pipe to the parent process */
		printf("MHELLOX");
		exit(0);
	}
	return 0;
}
