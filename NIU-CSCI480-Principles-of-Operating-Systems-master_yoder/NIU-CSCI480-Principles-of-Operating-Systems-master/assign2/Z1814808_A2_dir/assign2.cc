/****************************************************************
PROGRAM:   Assign
AUTHOR:    Spencer Yoder
LOGON ID:  Z1814808
DUE DATE:  Tuesday, September 18

FUNCTION: We will have three processes which communicate with 
	  each other using pipes.  The three processes will be
	  parent, child and grandchild. 
*****************************************************************/
#include<iostream>
#include<string>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;

//Function: that preforms the math needed for 
//	all process functions.
//Input: number
//Output: number after computations
int doMath(int num){
	return (4 * num + 3);	//Do the math
}

//Function: Parent work function that starts
//	the communcation and sends data to the child
//	over the pipe. It then recives data from the
//	grand child from the pipe.
//Input: Read and write ends of the pipe
//Output: none
void PWork(int writeStream, int readStream){
	string buffer = "1@";	//Strings for the input and output pipe data
	string value ="1";
	char ch;	//Used in reading in the pipe
	int m = 1;	//number to do math with

	cerr << "Parent        Value: " << m <<endl;	//Display first data
	write(writeStream, buffer.c_str(), buffer.length());	//send value to child
	buffer.clear();	//clear the strings
	value.clear();

	while(true){	//Keep reading things from the pipe
		while(read(readStream, &ch, 1) > 0){	//save anything that comes over the pipe
			if(ch == '@')	//If the end of the string stop reading
				break;
			value.push_back(ch);	//save char to string
		}

		try{
			m = stoi(value);	//try and convert the string to an int
		}catch(...){
			break;
		}

		if (m >= 99999999){	//the number is to large
			write(writeStream, "*@", 2);	//send exit to all other processes
			break;	//return to main function
		}

		else{
			m = doMath(m);	//calculate the math

			buffer = to_string(m);	//convert it to string
			cerr << "Parent        Value: " << m <<endl;	//display the value
			write(writeStream, buffer.c_str(), buffer.length());	//send it over the pipe
			write(writeStream, "@", 1);	//send the dilimiter
		}

		value.clear();	//clear the strings
		buffer.clear();
	}
}

//Function: Child work function that recives
//	a string over the pipe and converts
//	it into nubmers, does math, and them
//	converts it back to a string. It then
//	sends it back over the pipe.
//Input: Read and write ends of the pipe
//Output: none
void CWork(int writeStream, int readStream){
	string buffer;	//Input and output strings
	string value;
	char ch;	//char using in reading chars from the pipe
	int m;	//used in doing math

	while(true){
		while(read(readStream, &ch, 1) > 0){	//Read chars in from the pipe
			if(ch == '@')	//If char is delimiter
				break;	//last char in pipe
			value.push_back(ch);	//save in string
		}

		try{	
			m = stoi(value);	//convert to int
		}catch(...){
			break;
		}

		if (m >= 99999999){	//If number is too big
			write(writeStream, "*@", 2);	//send stop to other processes
			break;
		}

		else{
			m = doMath(m);	//calculate the new number

			buffer = to_string(m);	//conver to string
			cerr << "Child         Value: " << m <<endl;	//display it 
			write(writeStream, buffer.c_str(), buffer.length());	//send it over the pipe
			write(writeStream, "@", 1);	//send delimiter
		}

		value.clear();	//clear strings
		buffer.clear();
	}

}

//Function: GrandChild work function that recives
//	a string over the pipe and converts
//	it into nubmers, does math, and them
//	converts it back to a string. It then
//	sends it back over the pipe.
//Input: Read and write ends of the pipe
//Output: none
void GWork(int writeStream, int readStream){
	string buffer;	//strings for input and output
	string value;
	char ch;	//char for reading from the pipe
	int m;	//int for the math

	while(true){
		while(read(readStream, &ch, 1) > 0){	//Read single chars from the pipe
			if(ch == '@')	//If the delimiter stop
				break;
			value.push_back(ch);
		}

		try{
			m = stoi(value);	//convert string to int
		}
		catch(...){
			break;
		}

		if (m >= 99999999){	//If the number is too large
			write(writeStream, "*@", 2);	//send stop to other process
			break;
		}

		else{
			m = doMath(m);	//do the math on the number

			buffer = to_string(m);	//convert to string
			cerr << "GrandChild    Value: " << m <<endl;	//display it
			write(writeStream, buffer.c_str(), buffer.length());	//send it
			write(writeStream, "@", 1);	//send it
		}

		value.clear();	//clear it
		buffer.clear();	//bop it
	}	
}


//Main body function for the program
int main()
{
	int pipeA[2], pipeB[2], pipeC[2];	//3 pipes
	pid_t pid;	//somewhere to save the pid

	if(pipe(pipeA)){	//set up pipeA
		cerr << "Pipe A error!, Exiting now." <<endl;
		exit(-5);
	}

	if(pipe(pipeB)){	//set up pipeB
		cerr << "Pipe B error!, Exiting now." <<endl;
		exit(-5);
	}

	if(pipe(pipeC)){	//set up PipeC
		cerr << "Pipe C error!, Exiting now." <<endl;
		exit(-5);
	}





	pid = fork();	//Fork and make a child
	if(pid < 0){	//Bad fork
		cerr << "The Child fork failed!" <<endl;
		exit(-5);
	}

	else if(pid == 0){ //start of the child process
		pid_t g_pid;	//get ready for a grandchild
		g_pid = fork();	//and here is the grandchild

		if(g_pid < 0){	//Bad frok
			cerr << "The Grandchild fork failed!" <<endl;
			exit(-5);
		}

		else if (g_pid == 0){ //Grandchild process
			close(pipeC[0]);//Close unused ends of the pipe
			close(pipeB[1]);
			GWork(pipeC[1], pipeB[0]);	//Grandchild function
			close(pipeC[1]);	//Close remaining ends of the pipe
			close(pipeB[0]);
			exit(0);	//kill the process
		}

		else{//Rest of the child process
			close(pipeB[0]);	//close unused ens of the pipe
			close(pipeA[1]);
			CWork(pipeB[1], pipeA[0]);	//Do Child function
			close(pipeB[1]);	//close the used ends of the pipe
			close(pipeA[0]);
			exit(0); //fkill the process
		}

	}

	else{ //The rest of the parent process
		close(pipeA[0]); //Close Read end
		close(pipeC[1]); //Close Write end
		PWork(pipeA[1], pipeC[0]);
		close(pipeA[1]);	//close used pipes
		close(pipeC[0]);
		wait(0);	//wait for the rest to finish
		exit(0);	//kill the program
	}

	return 0;	//Just sitting here, because it is not needed
}
