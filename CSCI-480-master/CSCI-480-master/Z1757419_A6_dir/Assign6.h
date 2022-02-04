#ifndef ASSIGN6_H
#define ASSIGN6_H
/*******************************************************************************
 * File:        Assign6.h
 * Programmer:  Joseph McDade
 * zID:         1757419
 * Due Date:    11/14/2018
 *
 * Purpose:     Header file for Assign6 program
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <list>
#include <string>
#include <fstream>
#include <vector>
#include <string.h>

// Prototyping statements
void initialize_avail();
void readline(string);
void load_best(vector<string>&);
void load_first(vector<string>&);
void allocate_best(vector<string>&);
void allocate_first(vector<string>&);
void deallocate(string, string);
void terminate(string);
void print_avail();
void print_inuse();
void tokenize(string, vector<string>&);
void defrag();


// Set global constants
#define HOW_OFTEN 5
#define MAX_SIZE 4194304 // Max block size in bytes
#define MB 1048576 // MB in bytes

// Global variables
list<Memblock> avail, inuse;
bool best_fit = false;

#endif
