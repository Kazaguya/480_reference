#ifndef ASSIGN7_H
#define ASSIGN7_H
/*******************************************************************************
 * File:        Assign7.h
 * Programmer:  Joseph McDade
 * zID:         1757419
 * Due Date:    11/30/2018
 *
 * Purpose:     Header file for Assign7 program
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <list>
#include <string>
#include <fstream>
#include <vector>
#include <string.h>
#include <math.h>

// Prototyping statements
void initialize_directory();
bool readline(string);
bool copy_entry(string, string);
bool delete_entry(string);
bool new_entry(string, size_t);
bool modify_entry(string, size_t);
bool rename_entry(string, string);
int find_empty(int start = 0);
int allocate(size_t, int pass_start_block = 0);
int reallocate(size_t, int);
void deallocate(int);
int count_clusters(int);
bool exist_entry(string);
list<Entry>::iterator find_entry(string);
void tokenize(string, vector<string>&);
void print_directory();
void print_fat();
void print_entry(list<Entry>::iterator);

// Set global constants
#define HOW_OFTEN 6					// How often the simulation prints current status
#define ENTRIES_PRINT 240		// Entries of the FAT12 to print
#define ENTRIES_PERLINE 12	// Entries per line of the FAT12 to print
#define BLOCK_SIZE 512			// Size of block on disk in bytes
#define BLOCK_ENTRIES 12		// Entries a directory block can track

// Create global variables
short fat[4096] = {0};	// FAT12 table
list<Entry> directory;	// List of entries in the directory
	int sim_counter = 0;	// Tracks cycles of the simulation

#endif
