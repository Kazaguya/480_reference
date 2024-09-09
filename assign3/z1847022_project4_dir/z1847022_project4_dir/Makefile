#   Milena Zlaticanin - Z1847022     
#   CSCI 480 
#   Assignment 4
#   Due: 3/29/22

CC = g++
CXXFLAGS = -Wall -g

z1908120_project4: assign4.o process.o cpuScheduler.o
	g++ $(CXXFLAGS) -o z1908120_project4 assign4.o process.o cpuScheduler.o

process.o: process.cc
	g++ $(CXXFLAGS) -c -o process.o process.cc

cpuScheduler.o: cpuScheduler.cc
	g++ $(CXXFLAGS) -c -o cpuScheduler.o cpuScheduler.cc

clean:
	rm -f *.o z1908120_project4