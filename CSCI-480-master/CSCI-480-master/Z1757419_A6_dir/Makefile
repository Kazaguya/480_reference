#
# PROGRAM:    Assignment 6
# PROGRAMMER: Joseph McDade
# LOGON ID:   z1757419
# DATE DUE:   11/14/2018
#

# Compiler variables
CCFLAGS = -ansi -Wall -std=c++11

# Rule to link object code files to create executable file
Assign6: Assign6.o Memblock.o
	g++ $(CCFLAGS) -o Assign6 Assign6.o Memblock.o

# Rules to compile source code files to object code
Assign6.o: Assign6.cc Assign6.h Memblock.h
	g++ $(CCFLAGS) -c Assign6.cc

Memblock.o: Memblock.cc Memblock.h
	g++ $(CCFLAGS) -c Memblock.cc


# Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assign6
