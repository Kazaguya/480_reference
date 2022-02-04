#ifndef MEMBLOCK_H
#define MEMBLOCK_H
/*******************************************************************************
 * File:        Memblock.h
 * Programmer:  Joseph McDade
 * zID:         1757419
 * Due Date:    11/14/2018
 *
 * Purpose:     Header file for Memblock object class
 ******************************************************************************/
#include <iostream>
#include <string>

using namespace std;

class Memblock
{
  public:
    Memblock();
		Memblock(int, size_t);
		Memblock(int, size_t, string, string);

    //Accessor Methods
		//Get Methods
		inline int get_start() const { return(start); };
		inline int get_end() const { return(start + size); };
		inline size_t get_size() const { return(size); };
		inline string get_process_id() const { return(process_id); };
		inline string get_block_id() const { return(block_id); };

		//Set Methods
		inline void set_start(int new_start){ start = new_start; };
		inline void set_size(size_t new_size){ size = new_size; };
		inline void set_process_id(string new_pid){ process_id = new_pid; };
		inline void set_block_id(string new_bid){ block_id = new_bid; };
		void update(int, size_t);
		void update(int, size_t, string, string);

  private:
		int start;
		size_t size;
		string process_id;
		string block_id;

};

#endif
