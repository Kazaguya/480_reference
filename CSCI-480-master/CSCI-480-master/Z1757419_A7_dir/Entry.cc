/*******************************************************************************
 * File:        Entry.cc
 * Programmer:  Joseph McDade
 * zID:         1757419
 * Due Date:    11/30/2018
 *
 * Purpose:     Class definitions for the Entry object class
 ******************************************************************************/

#include "Entry.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  Constructors  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 * Default Constructor
 * Arguments: new_start - Starting block of assigned blocks
 * Notes:     Builds the entry
 ******************************************************************************/
Entry::Entry(int new_start){
	name = nullptr;
	size = 0;
	start_block = new_start;
}
/*******************************************************************************
	Constructor Overload
	Arguments:	new_name - Name of the entry being created
							new_start - Starting block of assigned blocks
*******************************************************************************/
Entry::Entry(string new_name, int new_start){
	name = new_name;
	size = 0;
	start_block = new_start;
}
/*******************************************************************************
	Constructor Overload
	Arguments:	new_name - Name of the entry being created
							new_size - Size of the entry being created
							new_start - Starting block of assigned blocks
*******************************************************************************/
Entry::Entry(string new_name, size_t new_size, int new_start){
	name = new_name;
	size = new_size;
	start_block = new_start;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Operator Overloads //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
Method:			Entry&	Entry::operator=(const Entry &rhs)
Use:				Sets an Entry equal to another Entry using the '=' operator
Arguments:	&rhs - The Entry to set the left side equal to
Returns:		The original calling Entry with updated values
*******************************************************************************/
Entry&	Entry::operator=(const Entry &rhs){
	this->name = rhs.name;
	this->size = rhs.size;
	this->start_block = rhs.start_block;
	return *this;
}
