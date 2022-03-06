all:main_seance_1 test_main

seance_1.o : seance_1.c seance_1.h
	gcc -Wall -ggdb -c seance_1.c -o seance_1.o -lm

main_seance_1.o: main_seance_1.c seance_1.h
	gcc -Wall -ggdb -c main_seance_1.c -o main_seance_1.o -lm

main_seance_1: main_seance_1.o seance_1.o
	gcc -Wall -ggdb -o main_seance_1 main_seance_1.o seance_1.o -lm
	
test_main.o: test_main.c seance_1.h
	gcc -Wall -ggdb -c test_main.c -o test_main.o -lm
	
test_main: test_main.o seance_1.o
	gcc -Wall -ggdb -o test_main test_main.o seance_1.o -lm

