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
        	fprintf(stdout,"%s No such file\n",fileName);
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

	char buff[LINE_LEN+1]; 					/*buffer for input max len is max len +1 for checking longer lines*/
	boolean error = false,passError = false, synError=false;/*error : if at least one error ,passError: if in the pass logic Error,synError: if in the syntasx Error*/
	int fIndex=0;/*file line index*/
	IC=100;	/*counter for memory place init 100 place*/
	DC=0;	/*counter for label place init 100 place*/
	while (fgets(buff,LINE_LEN+1, fp) != NULL ) /*read line to buffer*/
	{
		fIndex++;
		synError = !syntax_chack (buff, fIndex);
		if(!synError)/*if no syntax error*/	
			passError = !lineFirstPass(buff,fIndex);
		if((passError || synError) && !error)/*if an error has occured*/
			error =true;	
	}
	if(!error){/*if didnt recived error*/
		updateDataLabels(IC);
	}
	return !error;
}

/*input: FILE *fp : pointer to source file
output: boolean value if the function sucsses
the function sets unut values in the counters sets the fp pointer to the start of the file 
and preform the second pass of the assembler.
if an error hase occured retrun false else true*/
boolean secondPass(FILE *fp){

	char buff[LINE_LEN];			/*buffer for input*/
	boolean error = false,secError = false;	/*error: if there is an error in second pass,secError : error in line */
	int fIndex=0;				/*row counter*/
	fseek(fp,0,SEEK_SET);/*return to start of the file*/
	DC=IC;			/*end of instructions memory from first pass*/
	IC=MEMORY_START;	/*counter for memory place init 100 place*/
	while (fgets(buff,LINE_LEN, fp) != NULL ) 
	{
		fIndex++;
		secError = !lineSecondPass(buff,fIndex);
		if(!error && secError)/*if there was an error in row and no error until now*/	
			error = true;
	}
	updateRAMCounters();/*update the sum of counters*/
	return !error;
}










   
