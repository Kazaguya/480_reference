/*********************************************************************
   PROGRAM:    CSCI 480 Assignment 1
   PROGRAMMER: Joseph McDade
   LOGON ID:   z1757419
   DUE DATE:   09/07/2018

   FUNCTION:   This program is to practice using different system
               calls within a Linux OS
*********************************************************************/

#include <iostream>
#include <unistd.h>     // fork
#include <wait.h>       // wait
#include <stdlib.h>     // system

using namespace std;

int main() {
		// Disable buffer for cout
		cout << unitbuf;
    std::cout << "I am the original process with PID: " << getpid()
    << ", my parent has PID: " << getppid() << '\n';
    // Call first fork
    std::cout << "Calling first fork" << '\n';
    pid_t child = fork();
    if(child == -1){ // If the first fork fails
        std::cout << "First fork failed" << '\n';
    } else if (child == 0)
    { // If in the first fork, this is the child
        std::cout << "I am the child with PID: " << getpid()
            << ", my parent has PID: " << getppid() << '\n';
        // Call the second fork
        std::cout << "Calling second fork" << '\n';
        pid_t grandChild = fork();
        if(grandChild == -1){ // If the second fork fails
            std::cout << "Second fork failed" << '\n';
        } else if (grandChild == 0)
        { // If in the second fork, this is the grandchild
            std::cout << "I am the grandchild process with PID: " << getpid()
                << ", my parent has PID:  " << getppid() << '\n';
            // Exit the grandchild
            std::cout << "Grandchild about to exit" << '\n';
            exit(0);
        } else { // Else in child process
            std::cout << "I am the child with PID: " << getpid()
                << ", my parent has PID: " << getppid() << '\n';
            wait(0); // wait for grandchild to terminate
            std::cout << "Child about to exit" << '\n';
            exit(0);
        }
    } else { // Else in parent process
        std::cout << "I am the parent process with PID: " << getpid()
            << ", my prant has PID: " << getppid() << '\n';
        // Call ps using system
        std::cout << "About to call ps" << '\n';
        system("ps");
        wait(0); // wait for child to terminate
        std::cout << "Parent about to exit" << '\n';
        exit(0);
    }
    return 0;
}
