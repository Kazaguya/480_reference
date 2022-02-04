/*******************************************************************************
 * File:        Memblock.cc
 * Programmer:  Joseph McDade
 * zID:         1757419
 * Due Date:    11/14/2018
 *
 * Purpose:     Class definitions for the Memblock object class
 ******************************************************************************/

#include "Memblock.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  Constructors  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 * Default Constructor
 * Arguments: none
 * Notes:     Builds the default Memblock
 ******************************************************************************/
Memblock::Memblock(){
	start = 0;
	size = 0;
	process_id = "NULL";
	block_id = "NULL";
}
/*******************************************************************************
 * Overloaded Constructor
 * Arguments: new_start - The starting address for the Memblock
 *						new_size - The size of the Memblock
 * Notes:     Builds a Memblock with a given starting address and size
 ******************************************************************************/
Memblock::Memblock(int new_start, size_t new_size){
	start = new_start;
	size = new_size;
	process_id = "NULL";
	block_id = "NULL";
}
/*******************************************************************************
 * Overloaded Constructor
 * Arguments: new_start - The starting address for the Memblock
 *						new_size - The size of the Memblock
 *						new_pid - The PID for the Memblock
 *						new_bid - The Block ID for the Memblock
 * Notes:     Builds a Memblock with a given starting address, size, process ID
 *						and block ID.
 ******************************************************************************/
 Memblock::Memblock(int new_start, size_t new_size,
 	string new_pid, string new_bid){
		start = new_start;
		size = new_size;
		process_id = new_pid;
		block_id = new_bid;
	}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Set Methods /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
Method:			void Memblock::update(int new_start, size_t new_size)
Use:				Updates a Memblock with a new starting address and size
Arguments:	new_start - Starting address to update to
						new_size - New size to update to
Returns:		none
*******************************************************************************/
void Memblock::update(int new_start, size_t new_size){
	start = new_start;
	size = new_size;
}
/*******************************************************************************
Method:			void Memblock::update(int new_start, size_t new_size,
							string new_pid, string new_bid)
Use:				Updates a Memblock with a new starting address, size, PID, and Block
						ID
Arguments:	new_start - Starting address to update to
						new_size - New size to update to
						new_pid - New process ID to update to
						new_bid - New block ID to update to
Returns:		none
*******************************************************************************/
void Memblock::update(int new_start, size_t new_size,
	string new_pid, string new_bid){
	start = new_start;
	size = new_size;
	process_id = new_pid;
	block_id = new_bid;
}
