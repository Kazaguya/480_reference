#
# PROGRAM: Assignment 4
# AUTHOR:  Eriq Walker
# ZID:     Z1908120
# CLASS:   CSCI480
#

CC = g++
CXXFLAGS = -Wall -g

z1908120_project4: main.o process.o cpuScheduler.o
	g++ $(CXXFLAGS) -o z1908120_project4 main.o process.o cpuScheduler.o

# z1908120_project3.o: z1908120_project3.cpp
# 	g++ $(CXXFLAGS) -c -o z1908120_project3.o z1908120_project3.cpp
# 
# PQueue.o: PQueue.cpp
# 	g++ $(CXXFLAGS) -c -o PQueue.o PQueue.cpp
# 
process.o: process.cpp
	g++ $(CXXFLAGS) -c -o process.o process.cpp
# 
# Queue.o: Queue.cpp
# 	g++ $(CXXFLAGS) -c -o Queue.o Queue.cpp

cpuScheduler.o: cpuScheduler.cpp
	g++ $(CXXFLAGS) -c -o cpuScheduler.o cpuScheduler.cpp

clean:
	rm -f *.o z1908120_project4