all:prossescmd

prossescmd: DataStruct.o Syntax_Utiles.o Syntax.o prossescmd.o main.o
	gcc -ansi -Wall -pedantic -o prossescmd DataStruct.o Syntax_Utiles.o Syntax.o prossescmd.o main.o
	
DataStruct.o:  DataStruct.c DataStruct.h Constats.h 
	gcc -ansi -Wall -pedantic -c DataStruct.c -lm
	
Syntax_Utiles.o:  Syntax_Utiles.c Syntax.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c Syntax_Utiles.c -lm
		
Syntax.o:  Syntax.c Syntax.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c Syntax.c -lm
		
prossescmd.o: prossescmd.c prossescmd.h DataStruct.h Syntax.h  
	gcc -ansi -Wall -pedantic -c prossescmd.c -lm

main.o: main.c DataStruct.h Syntax.h prossescmd.h
	gcc -ansi -Wall -pedantic -c main.c -lm

clean:
	rm *.o *prossescmd

