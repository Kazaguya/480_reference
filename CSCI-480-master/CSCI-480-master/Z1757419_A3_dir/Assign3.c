/*******************************************************************************
   PROGRAM:			CSCI 480 Assignment 3
   PROGRAMMER:	Joseph McDade
   LOGON ID:		z1757419
   DUE DATE:		09/29/2018
	 Note:				Uses hard tabs set to display as 2 spaces

   FUNCTION:		Simulate a microshell where a user can input up to two commands
	 							with the output of one piped to the second using the '||' string
*******************************************************************************/

#include <stdio.h>
#include <stdbool.h>	// bool
#include <string.h>		// strtok
#include <unistd.h>		// fork, pipe
#include <wait.h>			// wait
#include <stdlib.h>		// malloc, realloc, free, exit

// Prototyping statements
bool find_pipe(char *input, char **output);
void tokenize(char *input, char **args);
void single_command(char **args);
void piped_command(char **args1, char **args2);

// Set global constants
#define INPUT_SIZE 256
#define READ 0
#define WRITE 1

int main() {
	// Create variables
	char *command[2];
	char input[INPUT_SIZE];
	bool pipe_found = false;
	// Disable stdout buffer
	setbuf(stdout, NULL);
	// Ask user for input / Prime the read
	printf("480shell> ");
	fgets(input, INPUT_SIZE, stdin);
	input[strlen(input)-1] = 0;
	while (strcmp(input, "q") != 0 && strcmp(input, "quit") != 0
	 	&& strcmp(input, "exit") != 0)
	{	// Keep shell open until user asks to quit
		pipe_found = find_pipe(input, command);	// Tokenize on pipe
		char **child1_args = malloc(1 * sizeof(child1_args[0]));
		tokenize(command[0], child1_args);	// Extract 1st command
		if(pipe_found)
		{	// If a pipe is found, create appropriate array and send to functions
			char **child2_args = malloc(1 * sizeof(child2_args[0]));
			tokenize(command[1], child2_args);	// Extract 2nd command
			piped_command(child1_args, child2_args);	// Run commands
			free(child2_args);
		} else { // No pipe found, execute only one command
			single_command(child1_args);
		}
		command[0] = command[1] = NULL;
		free(child1_args);
		// Ask user for new input
		printf("480shell> ");
		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input)-1] = 0;
	}
  return 0;
}
/////////////////////////////////  Functions  //////////////////////////////////
/*******************************************************************************
Function:		bool find_pipe(char *input, char **output)
Use:				Splits the input text on the "|" or "||" special character
Arguments:	input  - string to parse
						output - Array of two character pointer. First half of input is set
						to output[0] and if there is a second half it is set to output[1]
Returns:		Boolean value of true if a pipe was found and input was split
*******************************************************************************/
bool find_pipe(char *input, char **output){
	char *buffer = strtok(input, "|");
	output[0] = buffer;
	buffer = strtok(NULL, "|");
	if(buffer != NULL)
	{	// If there is a pipe save second command
		if(buffer[0] == '|') // If double pipe, remove second pipe
			buffer++;
		output[1] = buffer;
		return (true); // Return true, pipe found
	}
	return (false); // Return false, pipe not found
}
/*******************************************************************************
Function:		void tokenize(char *input, char **args, size_t *new_size)
Use:				Used to tokenize a string on spaces to prepare for use with execvp()
Arguments:	input    - string to tokenize
						args     - array of character pointers to place arguments in
						new_size - size of args array
Returns:		Nothing
*******************************************************************************/
void tokenize(char *input, char **args){
	int size = 0;
	args[size] = strtok(input, " ");
	while(args[size++] != NULL)
	{	// While not at end of string, continue tokenizing
		args = realloc(args, (size + 1) * sizeof(args[0]));
		args[size] = strtok(NULL, " ");
	}
	return;
}
/*******************************************************************************
Function:		void single_command(char **args)
Use:				Used to execute a command by itself
Arguments:	**args = A null terminated array of character pointers containing
						         the command and arguments you want to run
Returns:		Nothing
*******************************************************************************/
void single_command(char **args){
	pid_t child = fork();
	if(child < 0){	// Fork failed, print error
		fprintf(stderr, "Process failed to fork\n");
		exit(127);
	}
	if(child == 0)
	{	// Child Process
		if(execvp(args[0], args) < 0)
		{	// execvp failed, print error
			fprintf(stderr, "Command %s failed to run with execvp\n", args[0]);
			exit(127);
		}
		exit(0);
	}
	wait(0);
	return;
}
/*******************************************************************************
Function:		void piped_command(char **args1, char **args2)
Use:				Used to create a pipe between two commands. Sets stdout of the first
						command to the stdin of the second command
Arguments:	**args1 = Null terminated array of character pointers containing
						          the first command and arguments to run
						**args2 = Null terminated array of character pointers containing
						          the second command and arguments to run
Returns:		Nothing
*******************************************************************************/
void piped_command(char **args1, char **args2){
	int pipe_ends[2];
	if(pipe(pipe_ends) < 0){	// Pipe failed, print error
		fprintf(stderr, "Failed to open pipe\n");
		exit(127);
	}
	pid_t child1 = fork();
	if(child1 < 0){	// Child1 failed to fork, print error, exit
		fprintf(stderr, "Child 1 failed to fork\n");
		exit(127);
	}
	if(child1 == 0)
	{	// Child 1 Process
		close(WRITE);
		dup(pipe_ends[WRITE]);
		close(pipe_ends[READ]);
		close(pipe_ends[WRITE]);
		if(execvp(args1[0], args1) < 0)
		{	// execvp failed, print error
			fprintf(stderr, "Command %s failed to run with execvp\n", args1[0]);
			exit(127);
		}
		exit(0);
	}
	if(child1 > 0)
	{	// In parent, fork Child 2
		pid_t child2 = fork();
		if(child2 < 0){	// Child2 failed to fork, print error, exit
			fprintf(stderr, "Child 2 failed to fork\n");
			exit(127);
		}
		if(child2 == 0)
		{	// Child 2 Process
			close(READ);
			dup(pipe_ends[READ]);
			close(pipe_ends[WRITE]);
			close(pipe_ends[READ]);
			if(execvp(args2[0], args2) < 0)
			{	// execvp failed, print error
				fprintf(stderr, "Command %s failed to run with execvp\n", args2[0]);
				exit(127);
			}
			exit(0);
		}
	}
	// Parent Process
	close(pipe_ends[READ]);
	close(pipe_ends[WRITE]);
	wait(0);
	wait(0);
	return;
}
