#
# PROGRAM:    Assignment 7
# PROGRAMMER: Joseph McDade
# LOGON ID:   z1757419
# DATE DUE:   11/30/2018
#

# Compiler variables
CCFLAGS = -ansi -Wall -std=c++11

# Rule to link object code files to create executable file
Assign7: Assign7.o Entry.o
	g++ $(CCFLAGS) -o Assign7 Assign7.o Entry.o

# Rules to compile source code files to object code
Assign7.o: Assign7.cc Assign7.h Entry.h
	g++ $(CCFLAGS) -c Assign7.cc

Entry.o: Entry.cc Entry.h
	g++ $(CCFLAGS) -c Entry.cc


# Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assign7
