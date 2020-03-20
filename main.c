#include <stdio.h>
#include "prossescmd.h"

void writeFiles(char* fileName);

int main(int argc, char * argv[]){
	
	int i=0;
	if(argc==1)
		printf("no file recived\n");
	else{
		for(i=1;i<argc;i++){
			if(prossesAsm(argv[i]))
				writeFiles(argv[i]);	
			freeMemory();
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
	strcat(objFileN, fileName);
	strcat(objFileN, ".ob");
	strcat(extFileN, fileName);
	strcat(extFileN, ".ext");
	strcat(entFileN, fileName);
	strcat(entFileN, ".ent");
	/*writing object file*/
	fp = fopen(objFileN, "w+");
	fprintf(fp, "%d %d", memory -> instructionC, memory -> dataC);
	while (i < memory -> size) {
		fprintf(fp, "\n%d %05o", (memory->word +i)->address,(int)(memory->word +i)->data);
		i++;
	}
	fclose(fp);
	
	/*writing external labels file*/
	if (exT!=NULL) {
		fp = fopen(extFileN, "w+");
		i = 0;
		while (i < exT->size) {	    
    			fprintf(fp, "%s %d\n",(exT->exCall+i)->label,(exT->exCall+i)->address);
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


	printf("\nFile %s has been successfully compiled.\n", fileName);
	free(objFileN);
	free(extFileN);
	free(entFileN);


}
