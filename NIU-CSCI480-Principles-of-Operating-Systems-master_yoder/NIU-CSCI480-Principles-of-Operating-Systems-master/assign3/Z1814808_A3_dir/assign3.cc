/****************************************************************
PROGRAM:   Assign
AUTHOR:    Spencer Yoder
LOGON ID:  Z1814808
DUE DATE:  Tuesday, September 18

FUNCTION:  Microshell that will read an input, and pars it. If 
           There is a || in it, the program will run the first
	   command and then the second one, with the data piped
	   into it.
*****************************************************************/
#include<iostream>
#include <sys/wait.h>
#include <cstring>
#include <unistd.h>

using namespace std;


int main(void)
{
	char buf[1024];	//buffer to read in from the cli
	char quit[5] = "quit";	//cstring with quit it in
	int pipeA[2];	//space for the pipe ends
	int  statusA;	//current status of the fork
	int  statusB;	//status of the fork

	pid_t pidA;	//pid of forkA
	pid_t pidB;	//pid of forkB

	cerr<< "480shell> ";

	while (fgets(buf,1024,stdin) != NULL){	//Read in the user input
		int count = 0;	//var to step through the array
		char * ptr;	//pointer for parsing the cstring
		char * command[1024] = {NULL, NULL, NULL};	//space for the first command
		char * command2[1024] = {NULL, NULL, NULL};	//space for the second command
		bool needPipe = false;	//If the pipe operator was found

		buf[strlen(buf) -1] = 0;	//remove the lasst char


		ptr = strtok(buf, " ");	//start the search process

		if(buf[0] == 'q' || strcmp(buf, quit) == 0)	//If there is a single q or quit
			break;	//quit

		while(ptr != NULL){	//loop through the input
			if(strcmp( ptr, "||" ) == 0){	//If there is a pipe
				needPipe = true;	//set flag
				count = 0;	//reset array count
				ptr = strtok(NULL, " ");	//keep looking
			}

			if(!needPipe){	//If there is a pipe, add to the next string
				command[count] = ptr;	//add the char to the end
				ptr = strtok(NULL, " ");	//keep looking
				count++;	//couter++
			}
			else{	//If no pipe has been found
				command2[count] = ptr;	//add to c array 1
				ptr = strtok(NULL, " ");//keep looking
				count++;//add 1
			}
		}

		pipe(pipeA);//set up the pipe


		if ((pidA = fork()) == 0) {	//fork for the first child process
			if(needPipe){	//If there is a pipe
				dup2(pipeA[1], STDOUT_FILENO);	//set the pipe in STD out
				close(pipeA[0]);	//close the read end
			}

			execvp(command[0], command);	//excute the command
			cerr << "The First Command Failed" <<endl;	//you should never get here
			exit(127);	//exit
		}
		else if(pidA < 0){	//If the fork failed
			cerr<<"The First child fork failed!"<<endl;	//report it and exit
			exit(-5);
		}

		if(needPipe){	//If there was a pipe
			if ((pidB = fork()) == 0) {	//fork it
				dup2(pipeA[0], STDIN_FILENO);	//replace std input with the pipe
				close(pipeA[1]);	//close write end

				execvp(command2[0], command2);	//run command
				cerr << "The Second Command Failed" <<endl;	//if there was an error
				exit(127);	//exit
			}
			else if(pidB < 0){	//if the second pipe failed
				cerr<<"The Second child fork failed!"<<endl;
				exit(-5); //exit
			}
		}

		close(pipeA[0]);	//close all ends of the pipe
		close(pipeA[1]);	//close all ends of the pipe

		waitpid(pidA, &statusA, 0);	//wait for the processes
		waitpid(pidB, &statusB, 0);	//wait on the processes

		cerr<<"480shell> ";	//get ready to read next line
	}
	return 0;
} 
