/**************************************************************************
PROGRAMMER:   Branden Hidalgo
COURSE:       CSCI 480
SECTION:      2
INSTRUCTOR:   Harry Hutchins
TA:           Jingwan Li
PROGRAM:      Assignment 2
****************************************************************************/
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <stdio.h>

using namespace std;

void PWork(int, int);  //Initializing function for parent process
void CWork(int, int);  //Initializing function for child process
void GWork(int, int);  //Initializing function for grandchild process 

int main()
{
   int A[2];           //Declaring file descriptors for pipe A
   int B[2];           //Declaring file descriptors for pipe B
   int C[2];           //Declaring file descriptors for pipe C

   pipe(A);      //Call pipe A.
   pipe(B);      //Call pipe B
   pipe(C);      //Call pipe C
   if(pipe(A) < 0)   //Pipe A error check
   {
    cerr << "pipe #1 error";
    exit(-5);
   }

   if(pipe(B) < 0)   //Pipe B error check
   {
    cerr << "pipe #2 error";
    exit(-5);
   }
  
   if(pipe(C) < 0)  //Pipe C error check
   {
    cerr << "pipe #3 error";
    exit(-5);
   }

   pid_t N = fork();          //Call first fork. Store success status
    
   if(N == -1)                //Fork error check
    {
     cerr << "fork error";
     exit(-5);
    }

   else if(N == 0)            //If first fork successful, in child proccess
    {
     pid_t N2 = fork();       //Call another fork. Store success status
   
     if(N2 == -1)             //Fork error check
     {
       cerr << "pipe #2 error";
       exit(-5);
     }
     else if(N2 == 0)         //If second fork successful, in grandchild process
     {
       close(C[1]);           //Close ends of pipes we not in use
       close(B[0]);
       close(A[1]);
       close(A[0]);
     
       cerr << "The grandchild process is ready to proceed." << endl;

       GWork(C[0], B[1]);      //Grandchild calls GWork, reads from pipe C and writes to pipe B

       close(C[0]);            //Close ends of pipes after use
       close(B[1]);

       exit(0);                //Exit grandchild process
     }

      close(B[1]);            //Close ends of pipes not in use
      close(A[0]);
      close(C[1]);
      close(C[0]);
     
      cerr << "The child process is ready to proceed." << endl;

      CWork(B[0], A[1]);      //Child calls CWork, reads from pipe B and writes to pipe A

      close(B[0]);            //Close ends of pipes after use
      close(A[1]);

      wait(0);                //Child waits until grandchild terminates (avoids zombie)
      exit(0);                //Exit child process
   }	    
   else                      //If first fork returns N > 0, in parent process
    {
     close(A[1]);             //Close ends of pipes not in use
     close(C[0]);
     close(B[1]);
     close(B[0]);

     cerr << '\n';
     cerr << "The parent process is ready to proceed." << endl;
    
     PWork(A[0], C[1]);       //Parent calls PWork, reads from pipe A and writes to pipe C

     close(A[0]);             //Close ends of pipes after use
     close(C[1]);

     wait(0);                 //Parent waits until child terminates (avoids zombie)
     exit(0);                 //Exit parent process
    }

return 0;                 //End main function
}

/*************************************************************************************************
 Function: void Pwork(int readFd, int writeFd)
Arguments: Takes two integers which are file descriptors for the pipes used in parent process
  Returns: N/A
  Purpose: Allows communication between processes via pipes
    Notes: 
*************************************************************************************************/
void PWork(int readFd, int writeFd)
{
   char Buffer[15] = "";		//Declare char array Buffer, size=15, initialized as empty array
   char Value[15] = "1";		//Declare char array Value,  size=15, initialized with a "1"
   long int M = 1;                      //Declare long integer M, initial value = 1

  cerr << "Parent:       Value = " << Value << endl;	
   write(writeFd, Value, strlen(Value)+1);	//Write value to write-end of Pipe C
   
   while(M > -999999999 && M < 999999999)        //Outer loop
   {
    int i = 0;           //ensure one bite at a time                       
    do                                          //Inner loop
    {
     read(readFd, &Value[i], 1);    //read from appropriate pipefd to Value
    }
    while(Value[i++] != '\0');     //while not at end of Value

    M = atol(Value);              //convert string to long int

    M = 200 - 3 * M;              //arithmetic
    
    sprintf(Buffer, "%ld", M);   //convert long int to string and store in Buffer

    cerr <<"Parent: " << '\t' << "Value = " << Buffer << endl;  
    write(writeFd, Buffer, strlen(Buffer)+1);             //Write from Buffer to appropriate pipefd
   } 
}

/*************************************************************************************************
 Function: void Cwork(int readFd, int writeFd)
Arguments: Takes two integers which are file descriptors for the pipes used in child process
  Returns: N/A
  Purpose: Allows communication between processes via pipes
    Notes: 
*************************************************************************************************/
void CWork(int readFd, int writeFd)
{
   char Buffer[15] = "";		//Declare Buffer array size=15
   char Value[15] = {'1'};		//Declare Value array  size=15
   long int M = 1;                      //Declare long integer M initial value = 1

  while(M > -999999999 && M < 999999999)
   {
    int i = 0;        //ensure one bite at a time                          
    do
    {
     read(readFd, &Value[i], 1);          //read from appropriate pipefd to Value
    }
    while(Value[i++] != '\0');          //while not at end of Value

    M = atol(Value);                   //Convert string to long int and store in M

    M = 7 * M - 6;                      //perform aritmetic on M
    
    sprintf(Buffer, "%ld", M);         //convert long int M to string and store in Buffer

    cerr << "Child: "<< '\t' << '\t' << "Value = " << Buffer << endl;
    write(writeFd, Buffer, strlen(Buffer)+1);   //write from Buffer to appropriate pipefd
   }
}

/*************************************************************************************************
 Function: void Cwork(int readFd, int writeFd)
Arguments: Takes two integers which are file descriptors for the pipes used in grandchild process
  Returns: N/A
  Purpose: Allows communication between processes via pipes
    Notes: 
*************************************************************************************************/
void GWork(int readFd, int writeFd)
{
   char Buffer[15] = "";		//Declare Buffer array size=15
   char Value[15] = {"1"};		//Declare Value array  size=15
   long int M = 1;                      //Declare long integer M initial value=1

  while(M > -999999999 && M < 999999999)
   {
    int i = 0;             //ensure one bite at a time
    do
    {
     read(readFd, &Value[i], 1);          //read from appropriate pipefd into Value
    }
    while(Value[i++] != '\0');            //while not at end of Value

    M = atol(Value);                      //conver string in Value to long int and store in M

    M = 30 - 4 * M;                       //Perform arithmetic on M
    
    sprintf(Buffer, "%ld", M);           //convert M from long int to string and store in Buffer
    
   // write(writeFd, Buffer, strlen(Buffer)+1);
    cerr << "Grandchild: " << '\t' << "Value = " << Buffer << endl;
    write(writeFd, Buffer, strlen(Buffer)+1);  //write from Buffer into appropriate pipefd
   }
} 
