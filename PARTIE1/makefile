all:main_seance_1

seance_1.o : seance_1.c seance_1.h
	gcc -Wall -c seance_1.c -o seance_1.o -lm

main_seance_1.o: main_seance_1.c seance_1.h
	gcc -Wall -c main_seance_1.c -o main_seance_1.o -lm

main_seance_1: main_seance_1.o seance_1.o
	gcc -Wall -o main_seance_1 main_seance_1.o seance_1.o -lm

