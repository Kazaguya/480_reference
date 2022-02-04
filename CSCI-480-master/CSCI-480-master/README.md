# CSCI-480
This is a repository I will be using to store all my CSCI 480 Principles of Operating Systems coursework. I have initially made this private as I do not want others in the class using my work.

## Assign 1
This assignment is just practice using basic system commands in C++ that work on Linux based operating systems. Such commands include:

* `fork()`
* `wait()`
* `system()`

## Assign 2
This builds on what was learned in the first assignment and adds the usage of `pipe()`.

## Assign 3
A simulation of a microshell. The shell should give a prompt `480shell>` and accept two commands that can be piped using the special character `||`, example: `480shell> cat ourfile || grep DeKalb` should return the expected `DeKalb`.

Included "ourfile" is a plaintext file to test using the shell with.

## Assign 4
A simulation of processes moving through different queues on a single processor. Processes are read from a file where they are moved into an *entry* queue. Initially all processes that arrived at arrival time == 0 and while still less than then number of AT_ONCE processes get loaded into the priority queue *ready* (each priority queue first compares priority of the process and if equal priorities they look for process that has been waiting longest). After this initialization only one process arrives per cycle if their arrival time has been met and processes are less than AT_ONCE. If there is no *active* process, then look in the ready queue for a process, if no process is waiting, then the CPU idles. When the *active* process finishes its CPU burst, moveQueue is called and it checks if the process needs to go to the *input* queue or *output* queue. If there are no *iactive* process or *oactive*, then check *input* and *output* queue respectively. When a process is terminated statistics are printed and the process is added to the *terminated* queue for tracking purposes.

## Assign 5
A simulation of the *Producers and Consumers* problem. Using POSIX threads multiple producer thread will be created. Each thread creates a "widget" and places it in a buffer, represented by incrementing a counter. Multiple consumer threads are also created. Each consumer thread removes a widget from the buffer, represented by decrementing a counter. Reading the size of the buffer will be done using semaphores while the incrementing/decrementing will be controlled by a mutex.

## Assign 6
A simulation in memory management. In this assignment we assume we have a small computer with only 16MB of memory and that the OS has taken the initial 3MB. Initially we keep track of available memory starting from 3MB and having 5 blocks (1MB, 2MB, 2MB, 4MB, 4MB) totaling the remaining 13MB. The simulation then reads a file and allocates, loads, deallocates, and terminates memory as it progresses. One of two arguments can be passed to the program to indicate to use the *first-fit* or *best-fit* algorithm for assigning memory.
