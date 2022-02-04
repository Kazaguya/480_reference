/*********************************************************************
   PROGRAM:			CSCI 480 Assignment 2
   PROGRAMMER:	Joseph McDade
   LOGON ID:		z1757419
   DUE DATE:		09/18/2018
	 Note:				Uses hard tabs set to display as 2 spaces

   FUNCTION:		This program adds practice of using pipe() to relay
	 							information between processes. It uses three pipes:
								-> parent -pipeA-> child -pipeB-> grandchild -
								|                                            |
								--------------------pipeC--------------------
								Each process reads a number and performs a calculation
								on that number. The program is terminated after the
								number reaches the target value.
*********************************************************************/

#include <iostream>
#include <iomanip>			// setw
#include <unistd.h>     // fork, pipe
#include <wait.h>       // wait
#include <stdlib.h>     // system, atoi

using namespace std;

// Prototyping statements
void work(int write_fd[], int read_fd[], const char name[],
	bool init = false);
void printValue(const char proc[], char value[]);
void performMaths(char value[]);

// Set global constants
#define READ_FD  0
#define WRITE_FD 1
#define SIZE 10
#define TARGET 99999999

int main() {
	// Disable cout buffer
	cout << unitbuf;
  // Create file descriptors
  int par_fd[2], child_fd[2], grand_fd[2];
  // Create and check pipes for errors
  if(pipe(par_fd) == -1){
		std::cerr << "First pipe failed" << '\n';
		exit(-5);}
	if(pipe(child_fd) == -1){
		std::cerr << "Second pipe failed" << '\n';
		exit(-5);}
	if(pipe(grand_fd) == -1){
		std::cerr << "Third pipe failed" << '\n';
		exit(-5);}
  // Create child processes
  pid_t childPID = fork();
  // Check if error creating fork
  if(childPID == -1){
    std::cerr << "Child process failed to start" << '\n';
    exit(-5);
  }	else if(childPID > 0)
  { /********* Parent process *********/
		// Close unused pipeB
		close(child_fd[READ_FD]);
		close(child_fd[WRITE_FD]);
		const char *par = "Parent:";
		work(par_fd, grand_fd, par, true);
		exit(0);
  }	else { // Child process
    // Create grandchild process
    pid_t grandPID = fork();
    //Check if error creating fork
    if(grandPID == -1){
      std::cerr << "Grandchild process failed to start" << '\n';
      exit(-5);
    }	else if(grandPID > 0)
    { /********* Child process *********/
			// Close unused pipeC
			close(grand_fd[READ_FD]);
			close(grand_fd[WRITE_FD]);
			const char *child = "Child:";
			work(child_fd, par_fd, child);
			wait(0);
			exit(0);
    }	else { /********* Grandchild process *********/
			// Close unused pipeA
			close(par_fd[READ_FD]);
			close(par_fd[WRITE_FD]);
			const char *grand = "Grandchild:";
			work(grand_fd, child_fd, grand);
			exit(0);
    }
  }
  return 0;
}
/////////////////////////////////  Functions  //////////////////////////////////
/*******************************************************************************
Function:		void work(int write_fd[], int read_fd[], const char name[],
						bool init)
Use:				Prints the current value and performs the math operation
						4 * value + 3. If the calling process is the initial process, value
						is left set to 1. Otherwise the value is initialized by being
						received from a pipe.
Arguments:	write_fd	- File descriptor of pipe to write to
						read_fd		- File descriptor of pipe to read from
						name 			- Name of process calling function
						init			- True or false if the process is initializing the value
												to have the maths performed on. Defaults to false.
Returns:		Nothing
*******************************************************************************/
void work(int write_fd[], int read_fd[], const char name[],bool init) {
	close(write_fd[READ_FD]);	// Close read end of write pipe
	close(read_fd[WRITE_FD]);	// Close write end of read pipe
	// Prime the read
	char value[SIZE] = "1";
	if(!init){ // If not the initial process, receive receive value from pipe
		read(read_fd[READ_FD], value, SIZE);
	}
	while (atoi(value) < TARGET)
	{	//While value is still less than TARGET
		printValue(name, value);
		performMaths(value);
		write(write_fd[WRITE_FD], value, SIZE);
		read(read_fd[READ_FD], value, SIZE);
	} // Write large value to pipe so child processes can exit
	write(write_fd[WRITE_FD], value, SIZE);
	// Close pipes
	close(read_fd[READ_FD]);
	close(write_fd[WRITE_FD]);
}

/*******************************************************************************
Function:		void printValue(const char proc[], char value[])
Use:				Prints the calling process and the current value
Arguments:	proc	- Calling process
						value	- Current value
Returns:		Nothing
*******************************************************************************/
void printValue(const char proc[], char value[]) {
	std::cout << setw(14) << left << proc << "Value: " << value << '\n';
}

/*******************************************************************************
Function:		void	performMaths(char value[])
Use:				Performs 4 * current value + 3
Arguments:	value	- Current value
Returns:		Nothing
*******************************************************************************/
void	performMaths(char value[]){
	int newValue = atoi(value);
	newValue = 4 * newValue + 3;
	sprintf(value, "%d", newValue);
}
