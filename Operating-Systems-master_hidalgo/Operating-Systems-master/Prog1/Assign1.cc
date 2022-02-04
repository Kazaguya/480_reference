/*****************************************************************************
PROGRAMMER: Branden Hidalgo
PROGRAM:    Assignment 1
COURSE:     CSCI 480
SECTION:    2
TA:         Jingwan Li
DATE DUE:   01/24/20
PURPOSE:    This is the first assignment of CSCI 480. This program is used 
            to practice with processe, PID's and functions like fork() and system()
******************************************************************************/
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;


int main() {                                                    //in main function
cerr << "\nThis is the original process. It's PID is ";
cerr << getpid();                                       //get current process's process ID
cerr << " and it's parent's ID is ";
cerr << getppid();                                             //get parent's process ID
cerr << "." << endl;

cerr << "Calling the first fork." << endl << endl;
pid_t N = fork();                                              //fork function

if(N == -1)                                                    //if first fork fails
  {
   cerr << "The first fork failed.";
   return -1;                                                  //if fork fails return -1
  }

else if(N == 0)                                                //in child process
  {
   cerr << "This is the child process. It's process ID is ";
   cerr << getpid() << " and it's parent's PID is ";           //get current process's process ID
   cerr << getppid() << endl;                                  //get parent process's process ID
   cerr << "Calling the second fork:" << endl << endl;
   pid_t N2 = fork();
   if(N2 == -1)                                                //if second fork fails
     {
      cerr << "The second fork failed.";
      return -1;                                               //if fork fails, return -1       
     }
   else if(N2 == 0)                                            //in grandchild process
     {
      cerr << "This is the grandchild process. It's process ID is " << endl;
      cerr << getpid() << " and it's parent's PID is ";                               //get grandchild process's process ID
      cerr << getppid() << endl << endl;                                              //get child process's process ID
      sleep(3);                                                                       //Pause program for 3 seconds
      cerr << "This process is now an orphan process. It's PID is ";
      cerr << getpid() << " and it's parent's PID is " << getppid() << "." << endl;   //get grandchild's process ID
      cerr << "The system() function is about to execute the \"ps\" command." << endl << endl;  
      system("ps");                                                                   //execute ps (process status) command via system function
      cerr << endl << "Grandchild process is about to exit.";
      return 0;                                                                       //grandchild process exits with status of 0      
     }
   cerr << "This is the child process. It's process ID is ";
   cerr << getpid() << " and it's parent's PID is ";
   cerr << getppid() << endl;
   sleep(2);
   cerr << "I am the child, about to exit." << endl;
   return 0;                                                      //child process exits with status of 0
  }
else                                                              //in parent process
  {
    cerr << "This is the parent process. It's PID is ";
    cerr << getpid() << " and it's parent's PID is ";             //get the parent's process ID
    cerr << getppid() << endl;                                    //get the parent's parent's process ID
    sleep(2);                                                     //Pause program for 2 seconds
    cerr << "This is the parent, about to call ps command. The child should appear as a zombie." << endl;
    system("ps");                                                 //execute ps (process status) command via system function
    sleep(3);                                                     //Pause program for 3 seconds
    wait(0);                                                      //Wait for child to terminate
    cerr << "Having waited on the child, there is another call to the ps command." << endl;
    system("ps");                                                 //execute ps (process status) command via systemr function
    cerr << "This process is about to terminate.";
    return 0;                                                     //parent process exits with status of 0
  }

return 0;                                                         //program returns 0
}
