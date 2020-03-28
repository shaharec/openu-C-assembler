all:prossescmd

prossescmd: DataStruct.o Syntax_Utiles.o Syntax.o LineUtils.o FirstPassUtils.o SecondPassUtils.o prossescmd.o main.o
	gcc -ansi -Wall -pedantic -o prossescmd DataStruct.o Syntax_Utiles.o LineUtils.o  Syntax.o FirstPassUtils.o SecondPassUtils.o prossescmd.o main.o
	
DataStruct.o:  DataStruct.c DataStruct.h Constats.h 
	gcc -ansi -Wall -pedantic -c DataStruct.c -lm

LineUtils.o:  LineUtils.c LineUtils.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c LineUtils.c -lm
	
Syntax_Utiles.o:  Syntax_Utiles.c Syntax.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c Syntax_Utiles.c -lm
		
Syntax.o:  Syntax.c Syntax.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c Syntax.c -lm
	
FirstPassUtils.o: FirstPassUtils.c FirstPassUtils.h LineUtils.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c FirstPassUtils.c -lm
	
SecondPassUtils.o: SecondPassUtils.c SecondPassUtils.h LineUtils.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c SecondPassUtils.c -lm
			
prossescmd.o: prossescmd.c prossescmd.h DataStruct.h LineUtils.h FirstPassUtils.h SecondPassUtils.h Syntax.h
	gcc -ansi -Wall -pedantic -c prossescmd.c -lm

main.o: main.c DataStruct.h Syntax.h prossescmd.h
	gcc -ansi -Wall -pedantic -c main.c -lm

clean:
	rm *.o *prossescmd

