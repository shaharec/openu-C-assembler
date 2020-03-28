#include "Prossescmd.h"

/*input: file name 
output:if the assembler succesfully worked-true else false
the function gets the file name inishelize the unstruction/data counters
and call the first pass, if sucsess go to next pass */
boolean prossesAsm(const char* fileName)
{
	FILE *fp=NULL;
	IC=MEMORY_START;	/*counter for memory place init 100 place*/
	DC=0;	/*counter for label place init 0 place*/
	fp = fopen(fileName, "r");
    	if (fp == NULL){
        	printf("%s No such file\n",fileName);
        	return false;}
        else{
		if(firstPass(fp))
			if(secondPass(fp)){
				fclose(fp);
				return true;
			}
		fclose(fp);		
	return false;
	}
}
/*input: ponter to file- fp
output:	boolean value of succsees
the function sets the initial values of the memory counters
and preform the first pass.
in the first pass the function fill the lable table and checks the syntax of the file */
boolean firstPass(FILE *fp){

	char buff[LINE_LEN];
	boolean error = false,passError = false, synError=false;
	int fIndex=0;/*file line index*/
	IC=100;	/*counter for memory place init 100 place*/
	DC=0;	/*counter for label place init 100 place*/
	/*printf("************first pass************\n");print first pass*/
	while (fgets(buff,LINE_LEN, fp) != NULL ) /*read line to buffer*/
	{
		fIndex++;
		synError = !syntax_chack (buff, fIndex);
		if(synError && !error)
			error =true;
		if(!error)	
			passError = !lineFirstPass(buff,fIndex);
		if(passError && !error)
			error =true;	
	}
	if(!error){/*if didnt recived error*/
		updateDataLabels(IC);
		/*printLbTable();print labels table- for check*/
	}
	return !error;
}

/*input: FILE *fp : pointer to source file
output: boolean value if the function sucsses
the function sets unut values in the counters sets the fp pointer to the start of the file 
and preform the second pass of the assembler.
if an error hase occured retrun false else true*/
boolean secondPass(FILE *fp){

	char buff[LINE_LEN];
	boolean error = false;
	int fIndex=0;
	fseek(fp,0,SEEK_SET);/*return to start of the file*/
	DC=IC;			/*end of instructions memory from first pass*/
	IC=MEMORY_START;	/*counter for memory place init 100 place*/
	/*printf("************second pass************\n");print start of second line - for check*/
	while (fgets(buff,LINE_LEN, fp) != NULL && !error) 
	{
		fIndex++;
		error = !lineSecondPass(buff,fIndex);	
	}
	updateRAMCounters();/*update the sum of counters*/
	/*check functions-print tabels
	printExT();
	printEnT();*/
	return !error;
}










   
