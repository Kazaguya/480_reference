/****************************************************************
PROGRAM:   Assign1
AUTHOR:    Spencer Yoder
LOGON ID:  Z1814808
DUE DATE:  Friday, September 7

FUNCTION:  Program that uses fork(), getpid(), getppid(),
           wait() and system() to show what happens when
           they get called. It does 2 forks makeing a total of
           3 different processes.
*****************************************************************/
#include<iostream>
#include<unistd.h>
#include <sys/wait.h>
using namespace std;

int main()
{
	pid_t pid; //var to save the child pid

	cerr << "I am the original process. My PID is " << getpid() << " and my parent's PID is " << getppid() <<endl;
	cerr << "Now we have the first fork." <<endl;

	pid = fork();  //fork the parent to make the child
	if(pid < 0)  //If the fork failed, tell it and exit
	{
		cerr << "The First fork has failed!" <<endl; 
		exit(-1);
	}

	else if(pid == 0)  //If the fork worked, This is what the child fork does
	{
		pid_t g_pid; //var for the grand child pid

		cerr << "I am the child. My PID is " << getpid() << " and my parent's PID is " << getppid() <<endl;
		cerr << "Now we have the second fork." <<endl;

		g_pid = fork(); //fork the program again for the grand child

		if(g_pid < 0)	//If the fork went bad
		{
			cerr << "The second fork has failed!" <<endl; //say it and exit
			exit(-1);
		}

		else if (g_pid == 0)//If it was a good fork, this is what the grand child does
		{
			cerr << "I am the grandchild. My PID is " << getpid() << " and my parent's PID is " << getppid() <<endl;
			cerr << "I am the grandchild. I am going to exit" <<endl;
			exit(0); //fxit
		}
		else //what does child process does after the fork
		{
			cerr << "I am the child. My PID is " << getpid() << " and my parent's PID is " << getppid() <<endl;
			wait(0); //wait until the grand child process finishes
			cerr << "I am the child process. I am about to exit." <<endl;
			exit(0); //exit child process
		}     
	}

	else //what the parent process does after the first fork
	{  
		cerr << "I am the parent process. My PID is " << getpid() << " and my parend's PID is " << getppid() <<endl;
		sleep(2); //sleep 3 seconds
		cerr << "I am the parent, about to call 'ps'." <<endl;
		system("ps"); //call the system ps command
		wait(0); //wait for all child process to finish
		cerr << "I am the parent. I am about to terminate." <<endl;
		exit(0); //close
	}

	return 0;
}


