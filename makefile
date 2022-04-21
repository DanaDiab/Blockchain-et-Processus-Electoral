all:main_seance_1 test_main main_seance_2 test_main_2 main_seance_3 main_seance_4 main_seance_5 main_seance_6 main_fusion_HC main_seance_7 main_finale

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

seance_4.o : seance_4.c seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c seance_4.c -o seance_4.o -lm

main_seance_4.o: main_seance_4.c seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c main_seance_4.c -o main_seance_4.o -lm

main_seance_4: main_seance_4.o seance_4.o seance_3.o seance_2.o seance_1.o
	gcc -Wall -ggdb -o main_seance_4 main_seance_4.o seance_4.o seance_3.o seance_2.o seance_1.o -lm

seance_5.o : seance_5.c seance_5.h seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c seance_5.c -o seance_5.o -lm -lssl -lcrypto

main_seance_5.o: main_seance_5.c seance_5.h seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c main_seance_5.c -o main_seance_5.o -lm -lssl -lcrypto

main_seance_5: main_seance_5.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o
	gcc -Wall -ggdb -o main_seance_5 main_seance_5.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o -lm -lssl -lcrypto
	
seance_6.o : seance_6.c seance_6.h seance_5.h seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c seance_6.c -o seance_6.o -lm -lssl -lcrypto
	
main_seance_6.o : main_seance_6.c seance_6.h seance_5.h seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c main_seance_6.c -o main_seance_6.o -lm -lssl -lcrypto
	
main_seance_6: main_seance_6.o seance_6.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o
	gcc -Wall -ggdb -o main_seance_6 main_seance_6.o seance_6.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o -lm -lssl -lcrypto

main_fusion_HC.o : main_fusion_HC.c seance_6.h seance_5.h seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c main_fusion_HC.c -o main_fusion_HC.o -lm -lssl -lcrypto
	
main_fusion_HC: main_fusion_HC.o seance_6.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o
	gcc -Wall -ggdb -o main_fusion_HC main_fusion_HC.o seance_6.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o -lm -lssl -lcrypto

seance_7.o : seance_7.c seance_7.h seance_6.h seance_5.h seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c seance_7.c -o seance_7.o -lm -lssl -lcrypto
	
main_seance_7.o : main_seance_7.c seance_7.h seance_6.h seance_5.h seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c main_seance_7.c -o main_seance_7.o -lm -lssl -lcrypto
	
main_seance_7: main_seance_7.o seance_7.o seance_6.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o
	gcc -Wall -ggdb -o main_seance_7 main_seance_7.o seance_7.o seance_6.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o -lm -lssl -lcrypto

main_finale.o : main_finale.c main_seance_7.c seance_7.h seance_6.h seance_5.h seance_4.h seance_3.h seance_2.h seance_1.h
	gcc -Wall -ggdb -c main_finale.c -o main_finale.o -lm -lssl -lcrypto
	
main_finale: main_finale.o seance_7.o seance_6.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o
	gcc -Wall -ggdb -o main_finale main_finale.o seance_7.o seance_6.o seance_5.o seance_4.o seance_3.o seance_2.o seance_1.o -lm -lssl -lcrypto
