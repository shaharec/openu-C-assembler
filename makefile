all:prossescmd

prossescmd: DataStruct.o prossescmd.o main.o
	gcc -ansi -Wall -pedantic -o prossescmd DataStruct.o prossescmd.o main.o
	
DataStruct.o: DataStruct.c DataStruct.h 
	gcc -ansi -Wall -pedantic -c DataStruct.h DataStruct.c -lm
	

prossescmd.o: prossescmd.c prossescmd.h DataStruct.h  DataStruct.c 
	gcc -ansi -Wall -pedantic -c prossescmd.c -lm

main.o: main.c prossescmd.h prossescmd.h DataStruct.h  DataStruct.c
	gcc -ansi -Wall -pedantic -c main.c -lm

clean:
	rm *.o *~prossescmd

