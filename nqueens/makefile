nqueens: main.o
	g++ -std=c++11 -g -Wall main.o -o nqueens

main.o: main.cpp table.h table.cpp
	g++ -std=c++11 -g -Wall -c main.cpp

clean:
	rm *.o nqueens
