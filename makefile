all:main_seance_1 test_main main_seance_2 test_main_2 main_seance_3

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

seance_2.o : seance_2.c seance_2.h seance_1.h
	gcc -Wall -ggdb -c seance_2.c -o seance_2.o -lm

main_seance_2.o: main_seance_2.c seance_2.h seance_1.h
	gcc -Wall -ggdb -c main_seance_2.c -o main_seance_2.o -lm

main_seance_2: main_seance_2.o seance_2.o seance_1.o
	gcc -Wall -ggdb -o main_seance_2 main_seance_2.o seance_2.o seance_1.o -lm

test_main_2.o: test_main_2.c seance_2.h seance_1.h
	gcc -Wall -ggdb -c test_main_2.c -o test_main_2.o -lm
	
test_main_2: test_main_2.o seance_1.o seance_2.o
	gcc -Wall -ggdb -o test_main_2 test_main_2.o seance_1.o seance_2.o -lm

seance_3.o : seance_3.c seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c seance_3.c -o seance_3.o -lm

main_seance_3.o: main_seance_3.c seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c main_seance_3.c -o main_seance_3.o -lm

main_seance_3: main_seance_3.o seance_3.o seance_2.o seance_1.o
	gcc -Wall -ggdb -o main_seance_3 main_seance_3.o seance_3.o seance_2.o seance_1.o -lm
