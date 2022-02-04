/****************************************************************
PROGRAM:   Assign6
AUTHOR:    Spencer Yoder
LOGON ID:  Z1814808
DUE DATE:  Wednesday, November 14

FUNCTION:  Program that simulating memory management. It reads in 
from a file that has what steps need to be compleated for each 
block of memory. Load will make a new memory block for process, 
Allocate will save more memory for the Process ID, Deallocate 
will remove the one block, and Terminate will delete all of the 
processes memory.
 *****************************************************************/
#include<iostream>
#include<fstream>
#include<sstream>
#include<list>

using namespace std;

#define HOWOFTEN	5;	//How often to print the lines
const int KB = 1024;    //Consts
const int MB = 1048576;
int startAddress = 3 * MB;
bool bestFit; //what type to simulation to run

void Load(int id, int size, string name);	//Functions to work with the blocks
void Allocate(int id, int size, string name);
void Deallocate(int id, string name);
void Terminate(int id);

//Object for the memory block
struct mBlock{
	string name;
	int ID;
	int startAddress;
	int size;
};

list<mBlock> Avail;	//Linked list to save the memory blocks
list<mBlock> InUse;

int main(int argc, char *argv[])
{
	char argument; //var to save the argument
	int count = 0; //count the line nubmers
	string line;	//ling to read from the file
	//list<mBlock>::iterator iter;	//iterator for printing the linked list
	int total = 0;	//total size of the space

	ifstream infile;	//set up file to read in
	infile.open("data6.txt");

	//load starting memory blocks
	mBlock block0;
	block0.size = MB;
	block0.startAddress = startAddress;
	startAddress = startAddress + MB;
	Avail.push_back(block0);

	mBlock block1;
	block1.size = MB*2;
	block1.startAddress = startAddress;
	startAddress = startAddress + MB*2;
	Avail.push_back(block1);

	mBlock block2;
	block2.size = MB*2;
	block2.startAddress = startAddress;
	startAddress = startAddress + MB*2;
	Avail.push_back(block2);

	mBlock block3;
	block3.size = MB*4;
	block3.startAddress = startAddress;
	startAddress = startAddress + MB*4;
	Avail.push_back(block3);

	mBlock block4;
	block4.size = MB*4;
	block4.startAddress = startAddress;
	startAddress = startAddress + MB*4;
	Avail.push_back(block4);

	if(argc > 1) {	//Look at the argument
		argument = argv[1][0];//get the argument

		if (argument == 'b') {	//If b then best fit
			cout << "Starting Simulation with Best-Fit method " <<endl;
			bestFit = true;
		}
		else if(argument == 'f') {	//If F then first fit
			cout << "Starting Simulation with First-Fit method " <<endl;
			bestFit = false;
		}
		else {	//Invalid letter
			cout << "Invalid arguments!" <<endl;
			return -1;
		}
	}
	else {		//No argument given
		cout << "No argumetns Given!" <<endl;
		return -1;
	}

	//Print the current memory
	cout << "This is a memory-management simulation" <<endl <<endl;
	cout << "***************************" <<endl;
	cout << "--Available Memory--" <<endl;
	for (list<mBlock>::iterator iter = Avail.begin(); iter != Avail.end(); ++iter) { //go through the avail list
		cout << "Address: " << iter->startAddress << "       Size: " << iter->size <<endl;
		total = total + iter->size;
	}
	if (total == 0) {
		cout << "	NONE" <<endl;
	}
	cout << "Total size of the list is = " << total <<endl <<endl;
	total = 0;
	cout << "--In Use Memory--" <<endl;
	for (list<mBlock>::iterator iter = InUse.begin(); iter != InUse.end(); ++iter) { //print the in use list
		cout << "Address: " << iter->startAddress << "       Size: " << iter->size <<endl;
		total = total + iter->size;
	}
	if (total == 0) {
		cout << "	NONE" <<endl;
	}
	cout << "Total size of the list is = " << total <<endl;
	cout << "***************************" <<endl <<endl;

	while(getline(infile, line))	//Read the lines in the file
	{
		char type;
		int id;
		int size;
		string name;

		istringstream inLine(line);	//Read line


		if(line.at(0) == '?'){	//Check the type
			break;
		}
		else if (line.at(0) == 'L') {
			inLine >> type >> id >> size >> name;
			Load(id, size, name);
		}
		else if (line.at(0) == 'A') {
			inLine >> type >> id >> size >> name;
			Allocate(id, size, name);
		}
		else if (line.at(0) == 'T') {
			inLine >> type >> id;
			Terminate(id);
		}
		else if (line.at(0) == 'D') {
			inLine >> type >> id >> name;
			Deallocate(id, name);
		}

		count++;	//input coutner

		if (count == 5) { //After 5 inputs print memory status
			cout <<endl;
			cout << "***************************" <<endl;
			cout << "--Available Memory--" <<endl;
			for (list<mBlock>::iterator iter = Avail.begin(); iter != Avail.end(); ++iter) {	//print avail memory list
				cout << "Address: " << iter->startAddress << "       Size: " << iter->size <<endl;
				total = total + iter->size;
			}
			if (total == 0) {
				cout << "	NONE" <<endl;
			}
			cout << "Total size of the list is = " << total <<endl <<endl;
			total = 0;
			cout << "--In Use Memory--" <<endl;
			for (list<mBlock>::iterator iter = InUse.begin(); iter != InUse.end(); ++iter) {	//print the in use memory
				cout << "Address: " << iter->startAddress << "       Size: " << iter->size << "       Name: " << iter->name <<endl;
				total = total + iter->size;
			}
			if (total == 0) {
				cout << "	NONE" <<endl;
			}
			cout << "Total size of the list is = " << total <<endl;
			cout << "***************************" <<endl <<endl;
			count = 0;
		}
	}

	infile.close();	//Close the file
	cout << "Simulation Ended" <<endl <<endl;

	//Print the end memory info
	cout << "***************************" <<endl;
	cout << "--Available Memory--" <<endl;
	for (list<mBlock>::iterator iter = Avail.begin(); iter != Avail.end(); ++iter) {
		cout << "Address: " << iter->startAddress << "       Size: " << iter->size <<endl;
		total = total + iter->size;
	}
	if (total == 0) {
		cout << "	NONE" <<endl;
	}
	cout << "Total size of the list is = " << total <<endl <<endl;
	total = 0;
	cout << "--In Use Memory--" <<endl;
	for (list<mBlock>::iterator iter = InUse.begin(); iter != InUse.end(); ++iter) {
		cout << "Address: " << iter->startAddress << "       Size: " << iter->size <<endl;
		total = total + iter->size;
	}
	if (total == 0) {
		cout << "	NONE" <<endl;
	}
	cout << "Total size of the list is = " << total <<endl;
	cout << "***************************" <<endl <<endl;

	return 0;
}

//Function that loads the process in to the memory
//Takes the ID, Size and Name and makes a new block
void Load(int id, int size, string name) {
	bool check = false;
	int tempSize = 15 * MB;

	list<mBlock>::iterator avail_itter;	//iterators to loop throught the list
	list<mBlock>::iterator temp_itter;	//Iterator to save a temp location

	if (bestFit) {	//If best fit
		for (avail_itter = Avail.begin(); avail_itter != Avail.end(); ++avail_itter) { //Loop to find the best space
			if ((avail_itter->size - size) < tempSize && (avail_itter->size - size) > 0) {	//Check to see if it will fit
				tempSize = (avail_itter->size - size);	//If good save block in iter
				temp_itter = avail_itter;
				check = true;
			}
		}

		if (check) {	//If there is a space save it in the in Use list
			for (avail_itter = Avail.begin(); avail_itter != Avail.end(); ++avail_itter) {
				if (avail_itter->startAddress == temp_itter->startAddress) {
					avail_itter->size = avail_itter->size - size;
					startAddress = avail_itter->startAddress;
					avail_itter->startAddress = avail_itter->startAddress + size;

					//save it in, in use
					mBlock block;
					block.size = size;
					block.startAddress = startAddress;
					block.ID = id;
					block.name = name;
					InUse.push_front(block);

					cout << "LOAD: Loaded  ID " << id << ", Name " << name << ", Size " << size <<endl;	//Print it out
					return;
				}
			}
		}
		else {	//If there is not enought space
			cout << "##LOADING ERROR: Not enought space to make new task ID " << id <<endl;
			return;

		}
	}

	else {	//If First fit
		for (avail_itter = Avail.begin(); avail_itter != Avail.end(); ++avail_itter) { //Find the first open space
			if ((avail_itter->size - size) >= 0) {
				check = true;
				break;
			}
		}

		if (check) { //If there is space, save it
			avail_itter->size = avail_itter->size - size;	
			startAddress = avail_itter->startAddress;
			avail_itter->startAddress = avail_itter->startAddress + size;

			//save it in, In use
			mBlock block;
			block.size = size;
			block.startAddress = startAddress;
			block.ID = id;
			block.name = name;
			InUse.push_front(block);

			cout << "LOAD: Loaded  ID " << id << ", Name " << name << ", Size " << size <<endl;	//Print it out
			return;
		}
		else {	//Not enought space
			cout << "##LOADING ERROR: Not enought space to make new task ID " << id <<endl;
			return;
		}
	}
}

//Function the same as Load but for allocating
void Allocate(int id, int size, string name) {
	bool check = false;
	int tempSize = 15 * MB;

	list<mBlock>::iterator avail_itter;	//iterators to loop throught the list
	list<mBlock>::iterator temp_itter;	//Iterator to save a temp location

	if (bestFit) {	//If best fit
		for (avail_itter = Avail.begin(); avail_itter != Avail.end(); ++avail_itter) { //Loop to find the best space
			if ((avail_itter->size - size) < tempSize && (avail_itter->size - size) > 0) {	//Check to see if it will fit
				tempSize = (avail_itter->size - size);	//If good save block in iter
				temp_itter = avail_itter;
				check = true;
			}
		}

		if (check) {	//If there is a space save it in the in Use list
			for (avail_itter = Avail.begin(); avail_itter != Avail.end(); ++avail_itter) {
				if (avail_itter->startAddress == temp_itter->startAddress) {
					avail_itter->size = avail_itter->size - size;
					startAddress = avail_itter->startAddress;
					avail_itter->startAddress = avail_itter->startAddress + size;

					//save it in, in use
					mBlock block;
					block.size = size;
					block.startAddress = startAddress;
					block.ID = id;
					block.name = name;
					InUse.push_front(block);

					cout << "ALLOCATE: Loaded  ID " << id << ", Name " << name << ", Size " << size <<endl;	//Print it out
					return;
				}
			}
		}
		else {	//If there is not enought space
			cout << "##ALLOCATEING ERROR: Not enought space to make new task ID " << id <<endl;
			return;

		}
	}

	else {	//If First fit
		for (avail_itter = Avail.begin(); avail_itter != Avail.end(); ++avail_itter) { //Find the first open space
			if ((avail_itter->size - size) >= 0) {
				check = true;
				break;
			}
		}

		if (check) { //If there is space, save it
			avail_itter->size = avail_itter->size - size;
			startAddress = avail_itter->startAddress;
			avail_itter->startAddress = avail_itter->startAddress + size;

			//save it in, In use
			mBlock block;
			block.size = size;
			block.startAddress = startAddress;
			block.ID = id;
			block.name = name;
			InUse.push_front(block);

			cout << "ALLOCATE: Loaded  ID " << id << ", Name " << name << ", Size " << size <<endl;	//Print it out
			return;
		}
		else {	//Not enought space
			cout << "##ALLOCATEING ERROR: Not enought space to make new task ID " << id <<endl;
			return;
		}
	}
}

//Function that deallocats the process from memory
//It must match the id and name
void Deallocate(int id, string name){
	list<mBlock>::iterator tempIter;
	bool check = false;	//If found
	bool check2 = false;

	for(list<mBlock>::iterator inUse_itter=InUse.begin(); inUse_itter != InUse.end(); ++inUse_itter){	//Loop throught until you find the id
		if(inUse_itter->ID == id && inUse_itter->name == name){	//if found the ID
			mBlock block;	//Make a new open space block
			block.size = inUse_itter->size;	//get the block info
			block.startAddress = inUse_itter->startAddress;

			for (list<mBlock>::iterator avail_itter = Avail.begin(); avail_itter != Avail.end(); avail_itter++) {	//loop through the avail list
				if (avail_itter->startAddress > inUse_itter->startAddress) {	//save memory space back in to avail where it goes
					Avail.insert(avail_itter, block);	//insert into avail
					check2 = true;
					break;
				}
			}
			if (!check2) {	//else just put the space at the back
				Avail.push_back(block);
			}

			for (list<mBlock>::iterator avail_itter = Avail.begin(); avail_itter != --Avail.end(); avail_itter++) {	//Loop throught avail
				tempIter = avail_itter;	//Make a temp
				tempIter++;
				if ((avail_itter->size + avail_itter->startAddress) == (tempIter->startAddress)) {	//If temp starting address
					if ((avail_itter->size + tempIter->size) <= 4 * MB) {	//merge the space in the avail list
						avail_itter->size = avail_itter->size + tempIter->size;
						Avail.erase(tempIter);
						avail_itter--;
					}
				}
			}

			InUse.erase(inUse_itter);	//delete memory block
			inUse_itter--;
			check = true;
		}
	}

	if(!check){	//if it did not work
		cout << "##DEALLOCATE ERROR: Unable to find id " << id << " and name " << name <<endl;
	}
	else{	//If it worked print it
		cout << "DEALLOCATE: Removed id " << id << ", name " << name <<endl;
	}

}


//funtcion that will delete all memory for a process 
//and does not need the name.
void Terminate(int id){
	list<mBlock>::iterator tempIter;
	bool check = false;
	bool check2 = false;

	for(list<mBlock>::iterator inUse_itter=InUse.begin(); inUse_itter != InUse.end(); ++inUse_itter){	//Loop through and find the ID
		if(inUse_itter->ID == id){	//If the ID
			mBlock block;	//Make a new temp memory block
			block.size = inUse_itter->size;
			block.startAddress = inUse_itter->startAddress;

			for (list<mBlock>::iterator avail_itter = Avail.begin(); avail_itter != Avail.end(); avail_itter++) {	//Loop through the avail list
				if (avail_itter->startAddress > inUse_itter->startAddress) {	//Find where memory goes
					Avail.insert(avail_itter, block);
					check2 = true;
					break;
				}
			}
			if (!check2) {	//else save the memroy at the back
				Avail.push_back(block);
			}

			for (list<mBlock>::iterator avail_itter = Avail.begin(); avail_itter != --Avail.end(); avail_itter++) {	//Loop throught merge blocks
				tempIter = avail_itter;
				tempIter++;
				if ((avail_itter->size + avail_itter->startAddress) == (tempIter->startAddress)) {	//Find the block
					if ((avail_itter->size + tempIter->size) <= 4 * MB) {	//save the data
						avail_itter->size = avail_itter->size + tempIter->size;	//set the size
						Avail.erase(tempIter);	//delete it
						avail_itter--;
					}
				}
			}

			InUse.erase(inUse_itter);	//delete the memory block
			inUse_itter--;      
			check = true;
		}
	}

	if(!check){	//print if error
		cout << "##TERMINATE ERROR: Unable to find id " << id <<endl;
	}
	else{	//print if good
		cout << "TERMINATE: Removed id " << id <<endl;
	}
}
