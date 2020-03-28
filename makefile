all:Prossescmd

Prossescmd: DataStruct.o Syntax_Utiles.o Syntax.o LineUtils.o FirstPassUtils.o SecondPassUtils.o Prossescmd.o Main.o
	gcc -ansi -Wall -pedantic -o Prossescmd DataStruct.o Syntax_Utiles.o LineUtils.o  Syntax.o FirstPassUtils.o SecondPassUtils.o Prossescmd.o Main.o
	
DataStruct.o:  DataStruct.c DataStruct.h Constats.h 
	gcc -ansi -Wall -pedantic -c DataStruct.c -lm

LineUtils.o:  LineUtils.c LineUtils.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c LineUtils.c -lm
	
Syntax_Utiles.o:  Syntax_Utiles.c Syntax_Utiles.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c Syntax_Utiles.c -lm
		
Syntax.o:  Syntax.c Syntax.h Syntax_Utiles.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c Syntax.c -lm
	
FirstPassUtils.o: FirstPassUtils.c FirstPassUtils.h LineUtils.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c FirstPassUtils.c -lm
	
SecondPassUtils.o: SecondPassUtils.c SecondPassUtils.h LineUtils.h DataStruct.h 
	gcc -ansi -Wall -pedantic -c SecondPassUtils.c -lm
			
Prossescmd.o: Prossescmd.c Prossescmd.h DataStruct.h LineUtils.h FirstPassUtils.h SecondPassUtils.h Syntax.h
	gcc -ansi -Wall -pedantic -c Prossescmd.c -lm

Main.o: Main.c DataStruct.h Syntax.h Prossescmd.h
	gcc -ansi -Wall -pedantic -c Main.c -lm

clean:
	rm *.o *Prossescmd

