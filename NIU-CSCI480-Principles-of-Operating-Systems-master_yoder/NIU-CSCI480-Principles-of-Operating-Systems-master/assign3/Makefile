#  Spencer Yoder
.DEFAULT_GOAL := assign3.exe

assign3.o: assign3.cc
	g++ -c assign3.cc -Wall -std=c++11

assign3.exe: assign3.o
	g++ -o assign3.exe assign3.o -Wall -std=c++11

clean:
	rm *.o assign3.exe
