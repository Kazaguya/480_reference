#
# PROGRAM:    Assignment 1
# PROGRAMMER: Joseph McDade
# LOGON ID:   z1757419
# DATE DUE:   09/07/2018
#

# Compiler variables
CCFLAGS = -ansi -Wall -std=c++11

# Rule to link object code files to create executable file
Assign1: Assign1.o
	g++ $(CCFLAGS) -o Assign1 Assign1.o

# Rules to compile source code files to object code
Assign1.o: Assign1.cc
	g++ $(CCFLAGS) -c Assign1.cc

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assign1
