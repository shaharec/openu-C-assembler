all:prossescmd

prossescmd: DataStruct.o syntax.o prossescmd.o main.o
	gcc -ansi -Wall -pedantic -o prossescmd DataStruct.o syntax.o prossescmd.o  main.o 
	
DataStruct.o:  DataStruct.c DataStruct.h Constats.h 
	gcc -ansi -Wall -pedantic -c DataStruct.c -lm
	
syntax.o:  syntax.c syntax.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c syntax.c -lm
		
prossescmd.o: prossescmd.c prossescmd.h DataStruct.h syntax.h  
	gcc -ansi -Wall -pedantic -c  prossescmd.c -lm

main.o: main.c prossescmd.h
	gcc -ansi -Wall -pedantic -c main.c -lm

clean:
	rm *.o *~prossescmd

