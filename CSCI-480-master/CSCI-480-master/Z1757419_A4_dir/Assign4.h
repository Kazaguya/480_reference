#ifndef ASSIGN4_H
#define ASSIGN4_H
/*******************************************************************************
 * File:        Assign4.h
 * Programmer:  Joseph McDade
 * zID:         1757419
 * Due Date:    10/12/2018
 *
 * Purpose:     Header file for Assign4 program
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <fstream>
#include <string.h>

// Prototyping statements
queue<Process> buildEntryQueue();
void terminate_proc();
void moveQueue(Process*);
void printStats();

// Set global constants
#define MAX_TIME 500
#define AT_ONCE 5
#define HOW_OFTEN 25

int Timer = 0, process_amount = 0, cpu_idle = 0, terminated_total = 0;
Process *IActive = nullptr, *OActive = nullptr, *Active = nullptr;
// Create Queues
queue<Process> terminated, entry = buildEntryQueue();
priority_queue<Process> ready, input, output;

#endif
