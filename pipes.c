/*
    Filename:
        pipes.c

    Authors:
        Ian Goetting
        Hang Ngo

    Description:
        This program is our solution to problem 1 of the CS474 Programming Assignment #2.
        This program will create a child process and use a pipe to send a message from the
        child process to the parent process. The parent process will then reformat the message
        such that all the lowercase letters are made uppercase and all the uppercase letters
        are made lowercase. The parent process will then send the message back to the child via
        a second pipe. When the child process receives the formatted message, it prints it out.

        We treat the "first" process as the child process and the "second" process as the
        parent process. This means that the child process will take input from the user.    

*/
#include<string.h>
#include<unistd.h>
#include<sys/wait.h> 
#include<stdio.h>
#include<stdlib.h>
#define BUFFER_SIZE 256

//Function that formats the string
//i.e. turns all uppercase letters to lowercase letters and lowercase letters to uppercase letters
void formatString(char *str) {
	int i = 0;
	while(i < strlen(str)-1) {
		if(islower(str[i]))
			str[i] = toupper(str[i]);
		else
			str[i] = tolower(str[i]);
		i++;
	}
}

//Function that creates a child process and sends a message back to the parent process (via a pipe) so that 
//it can be formatted. The parent then sends the formatted message back to the child process (via a second pipe).
//The child process then prints out the formatted message.
void process_string(){
    char message[BUFFER_SIZE];
	int fd1[2]; // fd1[0] = read; fd1[1] = write
	int fd2[2]; // fd2[0] = read; fd2[1] = write
    //Create the pipes
	pipe(fd1);
	pipe(fd2);
    //Check the pipes were setup correctly.
	if(pipe(fd1)==-1 || pipe(fd2)==-1) {
		fprintf(stderr, "Pipe failed");
		exit(0);
	}
	pid_t pid = fork();
    //Check that the child process was created.
	if(pid < 0) {
		fprintf(stderr, "Fork failed");
		exit(0);
	}
    //Child process code
	if(pid == 0) {
		//close read to write
		close(fd1[0]);
		//Ask user to enter a message
		printf("Please enter a message: ");
		fgets(message,BUFFER_SIZE,stdin); 
        //Write the message to the pipe so it can be received by the parent.
		write(fd1[1],message,strlen(message)+1);
        //Read the formatted message sent from the parent and print it.
		read(fd2[0],message,BUFFER_SIZE);
		printf("Formatted message sent to the child process: %s\n",message);
		close(fd1[1]);
	}
    //Parent process code
	else {
		//close write to read
		close(fd1[1]);
        //Read the message sent from the child process.	
		read(fd1[0],message,BUFFER_SIZE);
        printf("\nMessage sent to the parent process: %s\n",message);
        //Re-format the string.
		formatString(message);
	    //Write the formatted message back to the child process
		write(fd2[1],message,strlen(message)+1);
		close(fd1[0]);
	}
}

int main() {
    process_string();
}

