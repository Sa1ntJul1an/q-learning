all: compile link

compile:
	g++ -c main.cpp
	g++ -c cell.cpp
	
link:
	g++ main.o cell.o search.o searchFactory.o breadthFirst.o depthFirst.o aStar.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lfreetype
