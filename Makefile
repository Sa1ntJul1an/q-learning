all: compile link

compile:
	g++ -c main.cpp
	g++ -c cell.cpp
	g++ -c car.cpp

link:
	g++ main.o cell.o car.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lfreetype
