#  Spencer Yoder
.DEFAULT_GOAL := assign5.exe

assign5.o: assign5.cc
	g++ -c assign5.cc -Wall -std=c++11

assign5.exe: assign5.o
	g++ -pthread -o assign5.exe assign5.o -Wall -std=c++11

clean:
	rm *.o assign5.exe
