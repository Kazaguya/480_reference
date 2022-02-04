#  Spencer Yoder
.DEFAULT_GOAL := assign4.exe

assign4.o: assign4.cc
	g++ -c assign4.cc -Wall -std=c++11

assign4.exe: assign4.o
	g++ -o assign4.exe assign4.o -Wall -std=c++11

clean:
	rm *.o assign4.exe
