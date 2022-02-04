/*******************************************************************************
   PROGRAM:			CSCI 480 Assignment 7
   PROGRAMMER:	Joseph McDade
   LOGON ID:		z1757419
   DUE DATE:		11/30/2018
	 Note:				Uses hard tabs set to display as 2 spaces

   FUNCTION:		Simulate a FAT12 filesystem
*******************************************************************************/

#include "Entry.h"
#include "Assign7.h"

int main() {
	ifstream file;
	file.open("data7.txt");
	if(file.fail()){
		cerr << "Failed to open file data7.txt" << endl;
		exit(-1);
	}
	string line;
	initialize_directory();
	cout << "Beginning of the FAT simulation" << endl;
	while(getline(file, line))
	{	// Read each line from the file for instructions
		if(sim_counter % HOW_OFTEN == 0){	// Print current status every HOW_OFTEN
			print_directory();
			print_fat();
		}
		if(!readline(line))
			break;	// If an error reading the line or at ?, exit the simulation
		sim_counter++;
	}
	cout << "Simulation finished in " << sim_counter << " steps" << endl;
	print_directory();
	print_fat();
  return 0;
}
/////////////////////////////////  Functions  //////////////////////////////////
/*******************************************************************************
Function:		void initialize_directory()
Use:				Initializes the directory list creating the '.' and '..' entries
Arguments:	none
Returns:		none
*******************************************************************************/
void initialize_directory(){
	// Create "." current directory
	new_entry(".", 512);
	// Create ".." parent directory
	new_entry("..", 0);
}
/*******************************************************************************
Function:		bool readline(string line)
Use:				Interprets a line from simulation file and runs the action
Arguments:	line - A line read from a file containing which actions for the
						simulation to take
Returns:		True on success, false on end of simulation marker ? and lines
						without proper instructions
*******************************************************************************/
bool readline(string line){
	vector<string> tokens;
	tokenize(line, tokens);
	if(tokens.size() == 0){	// If a blank line, print error and return false
		cerr << "Simulation encountered a blank line\n";
		return false;
	}
	switch(tokens[0][0]){	// Check line for function
		case 'C':	// Copy function
			cout << "Transaction:  Copy a file" << endl;
			if(copy_entry(tokens[1], tokens[2])){
				cout << "Successfully copied " << tokens[1]
					<< " to " << tokens[2] << endl;
			}
			return true;
		case 'D':	// Delete function
			cout << "Transaction:  Delete a file" << endl;
			if(delete_entry(tokens[1])){
				cout << "Successfully deleted a file, " << tokens[1] << endl;
			}
			return true;
		case 'N':	// New file function
			cout << "Transaction:  Add a new file" << endl;
			if(new_entry(tokens[1], stoi(tokens[2]))){
				cout << "Successfuly added a new file, " << tokens[1]
					<< ", of size " << tokens[2] << endl;
			}
			return true;
		case 'M':	// Modified file function
			cout << "Transaction:  Modify a file" << endl;
			if(modify_entry(tokens[1], stoi(tokens[2])))
				cout << "Successfully modified a file, " << tokens[1] << endl;
			return true;
		case 'R':	// Rename file function
			cout << "Transaction:  Rename a file" << endl;
			if(rename_entry(tokens[1], tokens[2])){
				cout << "Successfully changed the file name " << tokens[1]
					<< " to " << tokens[2] << endl;
			}
			return true;
		case '?':	// End of simulation
			cout << "\nEnd of the FAT simulation" << endl;
			return false;
		default:	// Invalid input
			cerr << tokens[0][0] << " not a valid simulation step" << '\n';
			return true;
	}
}
/*******************************************************************************
Function:		bool copy_entry(string old_entry, string copied_entry)
Use:				Copy one file to another with a new name
Arguments:	old_entry - File to copy
						copied_entry - New file to create from copy
Returns:		True on success, false on failure
*******************************************************************************/
bool copy_entry(string old_entry, string copied_entry){
	if(!(exist_entry(old_entry))){	// fail to copy, old_entry does not exist
		cerr << "File entry " << old_entry << " not found." << '\n';
		return false;
	} else if(exist_entry(copied_entry))
	{	// fail to copy, copied_entry already exists
		cerr << "Cannot copy entry, " << copied_entry << " already exists" << '\n';
		return false;
	} else {	// Copy old_entry to copied_entry
		list<Entry>::iterator it = find_entry(old_entry);
		new_entry(copied_entry, it->get_size());
		return true;
	}
}
/*******************************************************************************
Function:		bool delete_entry(string del_name)
Use:				Used to delete a directory entry and deallocate its blocks in FAT
Arguments:	del_name - The file name of the file to be deleted
Returns:		True on success, false on failure
*******************************************************************************/
bool delete_entry(string del_name){
	if(exist_entry(del_name)){	// If the file exists, delete it
		list<Entry>::iterator it = find_entry(del_name);
		deallocate(it->get_start());	// Deallocate the blocks in FAT
		directory.erase(it);	//  Remove the directory entry
		reallocate(BLOCK_SIZE*ceil((float)directory.size()/BLOCK_ENTRIES), 0);
		directory.begin()->resize	// Shrink the '.' block if needed
			(BLOCK_SIZE*ceil((float)directory.size()/BLOCK_ENTRIES));
		return true;
	} else {	// Failed to find file
		cerr << "File entry " << del_name << " not found." << '\n';
		return false;
	}
}
/*******************************************************************************
Function:		bool new_entry(string new_name, size_t new_size)
Use:				Create a new file
Arguments:	new_name - Name of the new file to be created
						new_size - The size of the file to be created
Returns:		True on success, false on failure
*******************************************************************************/
bool new_entry(string new_name, size_t new_size){
	// Check if already exists
	if(exist_entry(new_name)){	// Already exists, can't create
		cerr << "Cannot create new entry, " << new_name
		 	<< " already exists."<< '\n';
		return false;
	}	// Else, file doesn't exist, create it
	Entry new_entry_item(new_name, new_size,	// Create new entry item, allocate
		allocate(new_size, find_empty()));			// blocks, and find an empty start
	directory.push_back(new_entry_item);
	// Grow '.' if needed
	reallocate(BLOCK_SIZE*ceil((float)directory.size()/BLOCK_ENTRIES), 0);
	directory.begin()->resize
		(BLOCK_SIZE*ceil((float)directory.size()/BLOCK_ENTRIES));
	return true;
}
/*******************************************************************************
Function:		bool modify_entry(string mod_name, size_t new_size)
Use:				Resizing a file upon modification
Arguments:	mod_name - Name of file to modify
						new_size - New size of the modified file
Returns:		True on success, false on failure
*******************************************************************************/
bool modify_entry(string mod_name, size_t new_size){
	if(exist_entry(mod_name)){	// File to be modified exists
		list<Entry>::iterator it = find_entry(mod_name);
		// Make temp copy, remove from directory, create file in new location then
		// deallocate the old blocks
		Entry temp = *it;
		directory.erase(it);
		new_entry(temp.get_name(), new_size);
		deallocate(temp.get_start());
		return true;
	} else {	// Can't modify, file does not exist
		cerr << "File entry " << mod_name << " not found." << '\n';
		return false;
	}
}
/*******************************************************************************
Function:		bool rename_entry(string old_name, string new_name)
Use:				Renames the file while leaving the blocks in place, updates
						modified on timing
Arguments:	old_name - Name of file to rename
						new_name - Name to rename the file to
Returns:		True on success, false on failure
*******************************************************************************/
bool rename_entry(string old_name, string new_name){
	if(!(exist_entry(old_name))){	// Original file does not exist
		cerr << "File entry " << old_name << " not found." << '\n';
		return false;
	} else if(exist_entry(new_name)){	// New filename already taken
		cerr << "Can't rename entry, " << new_name << " already exists" << '\n';
		return false;
	} else {	// Rename entry
		list<Entry>::iterator it = find_entry(old_name);
		Entry temp = *it;
		directory.erase(it);
		temp.rename(new_name);
		directory.push_back(temp);
		return true;
	}
}
/*******************************************************************************
Function:		int find_empty(int start)
Use:				Finds an empty block in the FAT
Arguments:	start - Optional position to start searching for new block from
Returns:		Position of empty block or -1 if no free blocks found
*******************************************************************************/
int find_empty(int start){
	int found = -1;
	for(int empty = start; empty < 4096; empty++)
	{	// Search for empty block starting at start position
		if(fat[empty] == 0){
			found = empty;
			break;
		}
	}
	if(found == -1){	// Failed to find empty block
		cerr << "No free blocks found" << '\n';
		return -1;
	}	// else
	return found;
}
/*******************************************************************************
Function:		int allocate(size_t size, int pass_start_block)
Use:				Allocate blocks in FAT
Arguments:	size - Size of file to allocate space for
						start_block - Starting block to begin allocating space from
Returns:		Starting location of blocks or -1 if no blocks needed
*******************************************************************************/
int allocate(size_t size, int pass_start_block){
	int start_block = find_empty(pass_start_block);
	// Special case, size is 0
	if(size == 0){
		return -1;
	}
	// Else the size is greater than 0, find needed number of blocks
	int current_block = start_block;
	int needed_blocks = (ceil((float)size / BLOCK_SIZE));
	for(int i = 0; i < needed_blocks; i++){
		if(i == needed_blocks - 1)
		{	// If at the last block mark it end -1 and return
			fat[current_block] = -1;
			break;
		}
		fat[current_block] = -2; // Make current_block non empty so it isn't found
		current_block = fat[current_block] = find_empty(current_block);
	}
	return start_block;
}
/*******************************************************************************
Function:		int reallocate(size_t size, int start_block)
Use:				Reallocates the number of blocks used while keeping the blocks in
						place. If it needs more blocks, more are added on, if it needs less
						blocks are removed from the end.
Arguments:	size - New size of file
						start_block - Starting block of file to resize
Returns:		Starting location of blocks, or -1 if no blocks needed
*******************************************************************************/
int reallocate(size_t size, int start_block){
	// Special case, size is 0
	if(size == 0){
		deallocate(start_block);
		return -1;
	}
	if(start_block == -1){	// If previously empty, find a starting block
		start_block = find_empty();
	}
	// Initialize variables
	int allocated_blocks = count_clusters(start_block);
	int needed_blocks = (ceil((float)size / BLOCK_SIZE));
	int current_block = start_block;
	if(allocated_blocks == needed_blocks){	// No change in number of blocks
		return start_block;
	} else if(allocated_blocks < needed_blocks){	// Need more allocated blocks
		int i = 0;
		while(i++ < allocated_blocks - 1){	// Move to last currently assigned block
			current_block = fat[current_block];
		}
		current_block = fat[current_block] = find_empty();
		while(++i < needed_blocks){	// Allocate new blocks
			fat[current_block] = -1;	// Make current_block non-empty
			current_block = fat[current_block] = find_empty(current_block);
		}
		fat[current_block] = -1;	// Make last block -1 to terminate
		return start_block;
	} else {	// More space allocated than needed
		int i = 0;
		while(i++ < needed_blocks - 1){	// Move to end of needed blocks
			current_block = fat[current_block];
		}
		deallocate(fat[current_block]); // Free rest of of blocks
		fat[current_block] = -1;	// Make new last block -1 to terminate
	}
	return start_block;
}
/*******************************************************************************
Function:		void deallocate(int start_block)
Use:				Deallocated blocks from the FAT starting at a given block
Arguments:	start_block - The initial block to start deallocating from
Returns:		none
*******************************************************************************/
void deallocate(int start_block){
	if(start_block == -1){	// Size was 0, no blocks assigned to deallocate
		return;
	}
	int next_block;
	while(fat[start_block] != -1){	// While not at the last block
		next_block = fat[start_block];
		fat[start_block] = 0;	// Deallocate
		start_block = next_block;	// Move to next block
	}
	fat[start_block] = 0; // Deallocate the last block
}
/*******************************************************************************
Function:		int count_clusters(int start_block)
Use:				Get a count for the number of clusters in the list starting from a
						given block
Arguments:	start_block - The initial block to start counting from
Returns:		Integer number of blocks assigned
*******************************************************************************/
int count_clusters(int start_block){
	int count = 0;
	int current_block = start_block;
	if(start_block == -1)	// Size 0, no clusters
		return count;
	while(fat[current_block] != -1){	// While not at last cluster, count
		count++;
		current_block = fat[current_block];
	}
	return ++count;	// Count last cluster and return
}
/*******************************************************************************
Function:		bool exist_entry(string search_entry)
Use:				To find if a file exist in the directory
Arguments:	search_entry - File name to search for
Returns:		True if found, false if not found
*******************************************************************************/
bool exist_entry(string search_entry){
	bool found = false;
	list<Entry>::iterator it = directory.begin();
	while(it != directory.end()){	// Search through directory list
		if(it++->get_name() == search_entry){	// If found, stop searching
			found = true;
			break;
		}
	}
	return found;
}
/*******************************************************************************
Function:		list<Entry>::iterator find_entry(string search_entry)
Use:				Find where in the directory list a file is
Arguments:	search_entry - Filename to find
Returns:		iterator pointing to file in the directory list
*******************************************************************************/
list<Entry>::iterator find_entry(string search_entry){
	list<Entry>::iterator it = directory.begin();
	while(it != directory.end()){	// Search through directory
		if(it->get_name() == search_entry)	// When found, stop searching
			break;
		it++;
	}
	return it;
}
/*******************************************************************************
Function:		void tokenize(string line, vector<string> &tokens)
Use:				Tokenizes a line received as input
Arguments:	line - String to tokenize
						tokens - A vector to contain the tokens
Returns:		none
*******************************************************************************/
void tokenize(string line, vector<string> &tokens){
	if(line[line.size() - 1] == '\r')	// Remove carriage return if found
		line.erase(line.end()-1,line.end());
	char* token = strtok((char*)line.c_str(), " ");
	while(token)
	{	// Gather all tokens
		tokens.push_back((string)token);
		token = strtok(NULL, " ");
	}
}
/*******************************************************************************
Function:		void print_directory()
Use:				Prints the current contents of the directory list including each
						entry, entry's size, entry's clusters, total number of files and
						total size
Arguments:	none
Returns:		none
*******************************************************************************/
void print_directory(){
	cout << "\nDirectory Listing" << endl;
	int total_size = 0, total_files = 0;
	list<Entry>::iterator it = directory.begin();
	while(it != directory.end()){	// Print every directory entry
		print_entry(it);
		total_size += it++->get_size();
		total_files++;
	}
	cout << "Files:" << setw(3) << total_files
		<< "  Total Size:" << setw(7) << total_size << " bytes" << endl;
}
/*******************************************************************************
Function:		void print_fat()
Use:				Prints the FAT table with ENTRIES_PERLINE amount of entries on one
						line and ENTRIES_PRINT total amount of entries
Arguments:	none
Returns:		none
*******************************************************************************/
void print_fat(){
	cout << "\nContents of the File Allocation Table" << endl;
	int counter = 0, line_head = 0;
	cout << '#'  << setfill('0') << setw(3)<< line_head
		<< '-' << setw(3) << ENTRIES_PERLINE - 1;
	line_head += ENTRIES_PERLINE;
	while(counter < ENTRIES_PRINT)
	{	// Print ENTRIES_PRINT number of entries from the FAT
		if(!(counter % ENTRIES_PERLINE) && !(counter == 0))
		{	// If reached ENTRIES_PERLINE, start new line and line header
			cout << '\n' << '#'  << setfill('0') << setw(3)<< line_head
				<< '-' << setw(3) << line_head + ENTRIES_PERLINE - 1;
			line_head += ENTRIES_PERLINE;
		}
		cout << setfill(' ') << setw(6) << right << fat[counter++];
	}
	cout << '\n' << endl;
}
/*******************************************************************************
Function:		void print_entry(list<Entry>::iterator entry)
Use:				Prints information including name, size, and clusters of an entry
Arguments:	entry - List iterator for entry to be printed
Returns:		none
*******************************************************************************/
void print_entry(list<Entry>::iterator entry){
	int current_block = entry->get_start();
	cout << "File name: " << setw(23) << left << entry->get_name()
		<< "File Size: " << setw(6) << entry->get_size()
		<< setw(20) << "\nCluster(s) in use:  ";
	if(current_block == -1){	// If no blocks assigned, print "(none)"
		cout << setw(6) << right << "(none)";
	} else {	// Print a list of all clusters
		int i = 0;
		while(current_block != -1){	// Move through each cluster
			if((i % ENTRIES_PERLINE == 0) && !(i == 0)){	// If end of line, move to next line
				cout << endl << setw(26) << current_block;
			} else {
				cout << setw(6) << right << current_block;
			}
			i++;
			current_block = fat[current_block];
		}
	}
	cout << endl;
}
