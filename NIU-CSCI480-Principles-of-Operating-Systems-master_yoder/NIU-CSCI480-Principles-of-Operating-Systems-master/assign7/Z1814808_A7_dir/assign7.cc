/****************************************************************
PROGRAM:   Assign7
AUTHOR:    Spencer Yoder
LOGON ID:  Z1814808
DUE DATE:  Friday, November 30

FUNCTION:  Program that will maintaining a File Allocation Table 
		   and simulate "FAT-12" on a (very small) disk. Each
		   block will be 512 bytes in size. With FAT-12, we have
		   4096 blocks, so the disk is only 2 MB in size.
 *****************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream> 
#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <list>
#include <iomanip>

using namespace std;

const int HOWOFTEN = 6;	//Define some const
const int MAXBLOCK = 12;
const int MAXPRINT = 240;
const int MAXSIZE = 4096;
const int BLOCKSIZE = 512;


struct Entry{ //Entry Block for each file
	string name;
	int size;
	vector<int> blocks;
};

int FAT[MAXSIZE] = { };	//File allocation table array
vector<Entry> files;	//Vector to hold info about each file


//Function decleration
void Copy(string name, string name2);
void Delete(string name);
void New(string name, int size);
void Modify(string name, int size);
void Rename(string name, string name2);
int Search(string name);
void Print();


int main(int argc, char *argv[])
{
	int counter = 0;	//Counter for how ofter to print FAT

	ifstream file;	//Set up the read file
	file.open("data7.txt");	//Open it
	string line;


	New(".", 512);	//Make . and ..
	New("..",0);

	Print();	//Print the contents of the FAT


	while(!file.eof())	//Read the whole file
	{
		char type;	//transation type
		string name;
		string name2;
		int size;

		getline(file,line);	//Read line
		istringstream inLine(line);	//Parse it

		//Find what transations it best fit
		if(line.at(0) == 'C'){
			inLine >> type >> name >> name2;
			Copy(name, name2);
		}
		else if(line.at(0) == 'D'){
			inLine >> type >> name;
			Delete(name);
		}
		else if(line.at(0) == 'N'){
			inLine >> type >> name >> size;
			New(name, size);
		}
		else if(line.at(0) == 'M'){
			inLine >> type >> name >> size;
			Modify(name, size);
		}
		else if(line.at(0) == 'R'){
			inLine >> type >> name >> name2;
			Rename(name, name2);
		}
		else{
			break;
		}

		counter++;	//When the coutner it HOWOFTEN print the FAT
		if(counter == HOWOFTEN){
			Print();
			counter = 0;
		}
	}

	file.close();	//close the file

	Print();	//Print everything
	return 0;    
}

//Function: Copy one file and make a new one just
// like it, but in a different location. It checks
// to see if the new name is already used and that
// it can find the old one.
void Copy(string name, string name2){
	int index = Search(name);	//Call search to find the file
	int noDup = Search(name2);

	if(index < 0){	//the OG file is not found
		cout << "ERROR COPY: File " << name << " was not found." <<endl;
		return;
	}
	if(noDup > 0){	//If new name is found
		cout << "ERROR COPY: File " << name2 << " was already in use." <<endl;
		return;
	}

	New(name2, files.at(index).size);	//Call new funciton with the OG info
	cout << "COPY: File " << name << " has been copyed to " << name2 <<endl;

}

//Function: Deletes a file from the FAT after
// it check to see if thre was one.
void Delete(string name){
	int location = Search(name);	//find location

	if(location == -1){	//If there is no file with the name
		cout << "ERROR DELETE: File " << name << " was not found." <<endl;
		return;
	}
	else{	
		for (unsigned int i = 0; i < files.at(location).blocks.size(); i++) {	//delete the blocks in the FAT by setting them to 0
			FAT[files.at(location).blocks.at(i)] = 0;
		}

		files.erase(files.begin() + location);	//delete it out of the file vector

		cout << "DELETE: File " << name << " has beed deleted." <<endl;
	}
}

//Function: Make new files by adding them into
// the FAT and into the file vector. It check to
// see if the file is already in use.
void New(string name, int size){
	int amountOfBlocks;
	Entry tempFile;

	if (Search(name) != -1)	//Check to see if the file alreday is in use
	{
		cout << "ERROR NEW: File " << name << " is already in use." <<endl;
		return;
	}

	tempFile.name = name;	//Set data into temp file
	tempFile.size = size;

	if(size % BLOCKSIZE > 0 ){	//calc the Blocks needed, if there is a reaminder add 1
		amountOfBlocks = size / BLOCKSIZE;
		amountOfBlocks = amountOfBlocks + 1;
	}
	else{
		amountOfBlocks = size / BLOCKSIZE;
	}

	int count = 1;
	while(count <= amountOfBlocks){	//loop through the blocks that are needed
		for(int i=0; i < MAXSIZE; i++){     
			if(FAT[i] == 0 && count == amountOfBlocks){	//set the last block to -1
				FAT[i] = -1;
				tempFile.blocks.push_back(i);
				count++;
				break;
			}
			else if(FAT[i] == 0){	//set them to in use
				FAT[i] = 1;
				tempFile.blocks.push_back(i);
				count++;
				break;
			}
		}
	}
	
	files.push_back(tempFile);	//Make new file in the vector
	cout << "NEW: File " << name << " has been created." <<endl;
}


//Function: Modify a file that is already
// in the FAT and in the file vector. It
// Check to see if there is one already.
void Modify(string name, int size){
	int location = Search(name);	//get the file location

	if(location == -1){	//If there is no file with the name
		cout << "ERROR MODIFY: File " << name << " was not found." <<endl;
		return;
	}	
	else{	//Else add more blocks
		for(unsigned int i = 0; i < files.at(location).blocks.size(); i++)//set blocks to 0
			FAT[files.at(location).blocks.at(i)] = 0;
		files.erase(files.begin() + location );	//delete the file from the vector
		New(name, size);	//Make a new file 
		cout << "MODIFY: File " << name << " has been modifyed." <<endl;
	}
}

//Function: Renames a file to a new 
// name. It check both names before
// it does anything
void Rename(string name, string name2){
	int findName = Search(name);	//Check the OG name

	if(findName == -1){ //If the OG file is not found
		cout << "ERROR RENAME: File " << name << " was not found." <<endl; 
		return;
	}
	else if(Search(name2) >= 0){	//If the new file is already in use
		cout << "ERROR COPY: File " << name << " is already in use." <<endl;
		return;
	}
	else{
		files.at(findName).name = name2;	//Change the name in the vector
		cout << "RENAME: File " << name << " has been Renamed." <<endl;
	}
}

//Function: To find a name in the file
// vector.
int Search(string name){
	int count = 0;

	for(vector<Entry>::iterator iter = files.begin(); iter != files.end(); ++iter){	//Loop through the vector 
		if(iter->name == name){	//If found return the location
			return count;
		}
		count++;
	}
	return -1;	//else return -1 if not found
}

//Function: Print the contents of the 
// File allocation table and what blocks
// each file is using.
void Print(){
	int total = 0;

	cout << endl;
	cout << "Blocks occupied: " <<endl;

	int i = 0;
	while(i < MAXPRINT){	//Print the blocks
		cout << i << " - " << i+MAXBLOCK << ": \t";

		for(int j = 0; j <= MAXBLOCK; j++){	//Print the status of the block
			cout << FAT[j] << "\t";
		}
		i = i + MAXBLOCK;
		cout << endl;
	}
	cout<<endl;

	for(vector<Entry>::iterator iter = files.begin(); iter != files.end(); ++iter){	//Print the blocks each file are in
		cout << "File Name: " << iter->name << "\t File Size: " << iter->size << "\t Blocks Used: ";
		if(iter->blocks.size() == 0)	//If file does not take up any space, print none
			cout << "None";

		total = total + iter->size;
		
		for(unsigned int i = 0; i < iter->blocks.size(); i++){	//If the 
			cout << iter->blocks.at(i) << " ";
		}
		cout <<endl;
	}

	cout << "Total number of files: " << files.size() - 1 << "\t Total Size: " << total <<endl <<endl;
}
