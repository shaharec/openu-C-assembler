#include <stdio.h>
#include "Prossescmd.h"

void writeFiles(char* fileName);/*export the assembler tables to files*/

int main(int argc, char * argv[]){
	
	int i=0;
	char *fileName=NULL;
	/**char* asFile; = malloc((strlen(fileName) + strlen(".as") + 1)*sizeof(char));*/
	if(argc==1)
		fprintf(stdout,"no file recived\n");
	else{
		for(i=1;i<argc;i++){/*for every file entered*/
			fileName = malloc((strlen(argv[i]) + strlen(".as") + 1)*sizeof(char));
			strcpy(fileName, argv[i]);
			strcat(fileName, ".as");/*creat file name with .as ending*/
			fprintf(stdout,"**********Prossesing file %s**********\n",fileName);
			if(prossesAsm(fileName))/*activate assembler*/
				writeFiles(argv[i]);	/*if success export to files*/
			else fprintf(stdout,"**********File %s ended with error.**********\n\n",fileName);
			freeMemory();/*free allocatef memory of the assembler*/
			free(fileName);/*free string of file name*/
		}
	}
	return 1;
}

/* if compilation successfull will be called this method to write output file*/
void writeFiles(char* fileName){
    
	FILE *fp;
	char* objFileN = malloc((strlen(fileName) + strlen(".ob") + 1)*sizeof(char));
	char* extFileN = malloc((strlen(fileName) + strlen(".ext") + 1)*sizeof(char));
	char* entFileN = malloc((strlen(fileName) + strlen(".ent") + 1)*sizeof(char));
	int i = 0;
	strcpy(objFileN, fileName);
	strcat(objFileN, ".ob");
	strcpy(extFileN, fileName);
	strcat(extFileN, ".ext");
	strcpy(entFileN, fileName);
	strcat(entFileN, ".ent");
	
	if (memory!=NULL){/*writing object file*/
		fp = fopen(objFileN, "w+");
		fprintf(fp, "%d %d", memory -> instructionC, memory -> dataC);
		while (i < memory -> size) {
			fprintf(fp, "\n%04d %05o", (memory->word +i)->address,(int)(memory->word +i)->data);
			i++;
		}
		fclose(fp);
	}
	
	/*writing external labels file*/
	if (exT!=NULL) {
		fp = fopen(extFileN, "w+");
		i = 0;
		while (i < exT->size) {	    
    			fprintf(fp, "%s %04d\n",(exT->exCall+i)->label,(exT->exCall+i)->address);
    			i++;
		}
		fclose(fp);
	}
	/*writing entry labels file*/
	if (enT!=NULL) {
		fp = fopen(entFileN, "w+");
		i = 0;
		while (i < enT -> size) {
			fprintf(fp, "%s %d\n",(enT->line+i)->label,(enT->line+i)->address);
	   		 i++;
		}
		fclose(fp);
	}


	fprintf(stdout,"**********File %s.as has been successfully compiled.**********\n\n", fileName);
	free(objFileN);
	free(extFileN);
	free(entFileN);


}
