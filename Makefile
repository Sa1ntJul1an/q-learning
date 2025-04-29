all: compile link

compile:
	g++ -c main.cpp
	g++ -c cell.cpp
	g++ -c car.cpp
	g++ -c position.cpp
	g++ -c q-learner.cpp

link:
	g++ main.o cell.o car.o position.o q-learner.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lfreetype
