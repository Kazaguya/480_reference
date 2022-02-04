#
# PROGRAM:    Assignment 3
# PROGRAMMER: Joseph McDade
# LOGON ID:   z1757419
# DATE DUE:   09/29/2018
#

# Rule to link object code files to create executable file
Assign3: Assign3.o
	gcc -o Assign3 Assign3.o

# Rules to compile source code files to object code
Assign3.o: Assign3.c
	gcc -c Assign3.c

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assign3
