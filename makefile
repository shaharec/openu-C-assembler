all:prossescmd

prossescmd: DataStruct.o prossescmd.o
	gcc -ansi -Wall -pedantic -o prossescmd DataStruct.o prossescmd.o
	
DataStruct.o: DataStruct.c DataStruct.h 
	gcc -ansi -Wall -pedantic -c DataStruct.h DataStruct.c -lm
	

prossescmd.o: prossescmd.c prossescmd.h DataStruct.h  DataStruct.c 
	gcc -ansi -Wall -pedantic -c prossescmd.c -lm

clean:
	rm *.o *~prossescmd

