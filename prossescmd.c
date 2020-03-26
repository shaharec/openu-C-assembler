#include "prossescmd.h"

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
        	printf("no file found\n");
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
	int fIndex=0;
	IC=100;	/*counter for memory place init 100 place*/
	DC=0;	/*counter for label place init 100 place*/
	printf("************first pass************\n");
	while (fgets(buff,LINE_LEN, fp) != NULL ) 
	{
		fIndex++;
		synError = !syntax_chack (buff, fIndex);
		if(synError)
			error =true;
		if(!error)	
			passError = !lineFirstPass(buff,fIndex);
		if(passError)
			error =true;	
	}
	if(!error){
		updateDataLabels(IC);
		printLbTable();/*print labels table*/
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
	printf("************second pass************\n");
	while (fgets(buff,LINE_LEN, fp) != NULL && !error) 
	{
		fIndex++;
		error = !lineSecondPass(buff,fIndex);	
	}
	updateRAMCounters();/*update the sum of counters*/
	printExT();
	printEnT();
	return !error;
}

/*input: FILE *fp : pointer to source file
output: boolean value if the function sucsses
the function sets unut values in the counters sets the fp pointer to the start of the file 
and preform syntax check of the file and first pass of the assembler.
if an error hase occured retrun false else true*/
boolean lineSecondPass(char* line, int findex){
	
	lineWords *words = NULL;/*a pointer to a structer that contain the words in array*/
	boolean notEmpty,error=false;
	words = malloc(sizeof(lineWords));
	allcERR(words);     	
	words->size = 0;/*find first word in line*/
    notEmpty = getNextWordInLine(line,words);/*check if the line is not empty, if not save first word to fword*/
    if(notEmpty){/*if the row isnt emty (contain ' ' or tab only)*/
        	if(Islabel(words->word->str)){/*if the first word is a label*/
          	  	if(getNextWordInLine(line,words)){/*if there is another word*/
            			if(isCmd((words->word+words->size-1)->str)!=-1){/*check if ist a command*/
            				error = !handleRAMWords(INST,line,words);/*update RAM memory for a instruction row*/
            			}else{/*not command after label*/
            				if(strcmp((words->word+words->size-1)->str,DATA_CMD)==0 || strcmp((words->word+words->size-1)->str,STR_CMD)==0){/*if string or data*/
           						error = !handleRAMWords(DATA,line,words);/*update RAM memory for a instruction row*/
            				}else	if(strcmp((words->word+words->size-1)->str,ENT_CMD)==0){/*check if .entry command*/
            							if(getNextWordInLine(line,words))/*get lable after entry*/
            								error = !(addToEnT((words->word+words->size-1)->str));/*add the lable after to enry lable*/
            					}else {	error=true;/*unknown word*/
            						printf("error: unknown word: %s",(words->word+words->size-1)->str);
            						}
            			}
			}		   
			}else if(isCmd((words->word+words->size-1)->str)!=-1){/*check if command*/
            			error = !handleRAMWords(INST,line,words);/*update RAM memory for a instruction row*/
            	}else if(strcmp((words->word+words->size-1)->str,DATA_CMD)==0 || strcmp((words->word+words->size-1)->str,STR_CMD)==0){/*if string or data*/
           			error = !handleRAMWords(DATA,line,words);/*update RAM memory for a data or string row*/
            	}else if(strcmp((words->word+words->size-1)->str,ENT_CMD)==0){/*if it is entry row*/
            		if(getNextWordInLine(line,words))/*next word should be a lable*/
            			error = !(addToEnT((words->word+words->size-1)->str));/*add lable to entry table*/
            		}else if(strcmp((words->word+words->size-1)->str,EXT_CMD)!=0){/*not external line*/
            			printf("line: %d unknown word: %s line",findex,(words->word+words->size-1)->str); 
            			error = true;
            		}
            	freeWords(words);
           }
	return !error;/*retun no error has occurd in line*/	
}

/*input:char *line 	: 	raw line from file
	lineWords *words: 	an array of words fill with line words until INSTRUCTION command
output: boolean value	: 	succesfully update RAM and external table
the function update tha RAM and external table accourding INST line type*/
boolean InstRAMWords(char *line,lineWords *words){
		
	unsigned int RAMWord[MAX_WORD_INST]={0};
	int i=0,numWords=1;/*at least one word*/
	int reg=0;
	labelAd *label=NULL;
	
	RAMWord[0]|=(isCmd((words->word+words->size-1)->str)<<OCAM_SBIT);/*add opcode */
	RAMWord[0]|=A_THREE;/*turn on the A bit from ARE*/ 
       	if(getNextWordInLine(line,words)){
      		switch (*((words->word+words->size-1)->str))
			{
				case '#':
				RAMWord[1]=getDirectWord((words->word+words->size-1)->str);/*get the word for # operand*/ 
				numWords++;
				if(getNextWordInLine(line,words)){
					RAMWord[0]|=(A_ONE<<FOAM_SBIT);/*adress methode 0 place in 7 bit*/
					numWords++;
					label=labelExist((words->word+words->size-1)->str);/*check if second operand is label*/
					if(label!=NULL){
						RAMWord[0]|=(A_TWO<<SOAM_SBIT);
						RAMWord[2]=getLabelWord(label);
						if(label->labelType==EX_LABEL)
							addToExT(label->label,IC+numWords-1);
					}else {	reg = isReg((words->word+words->size-1)->str);
							if(reg!=-1){
								RAMWord[0]|=(A_FOUR<<SOAM_SBIT);
								RAMWord[2] = getRegWord(reg,SOAM_SBIT);
							}else if(*((words->word+words->size-1)->str)=='*'){
									reg = isReg((words->word+words->size-1)->str+1);
									if(reg!=-1){
											RAMWord[0]|=(A_THREE<<SOAM_SBIT);
											RAMWord[2] = getRegWord(reg,SOR_SBIT);
									}else{ 	printf("error: wrong input * before reg\n");
											return false;
										}
							}else {	printf("error: %s unknown operator\n",(words->word+words->size-1)->str);
									return false;
									}
						}
				}else RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode 0 place in second operand bit*/
				break;
				
				case 'r':
				numWords++;
				reg = isReg((words->word+words->size-1)->str);
				if(reg!=-1){/*check if its a register*/
					RAMWord[1] = getRegWord(reg,FOR_SBIT);	
				}else {
					label=labelExist((words->word+words->size-1)->str);/*check if secind operand is label*/
					if(label!=NULL){/*check if its a label*/
						RAMWord[1]=getLabelWord(label);
						if(label->labelType==EX_LABEL)
							addToExT(label->label,IC+numWords-1);	
					}else {	printf("error: unknown operand\n");
							return false;
							}
				}			
				if(getNextWordInLine(line,words)){
					if(reg != -1)
						RAMWord[0]|=(A_FOUR<<FOAM_SBIT);
					else if(label!=NULL)
						RAMWord[0]|=(A_TWO<<FOAM_SBIT);	
					if(*((words->word+words->size-1)->str)=='#'){
						RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode 0 place in 3 bit*/
						RAMWord[2]=getDirectWord((words->word+words->size-1)->str);/*get the word for # operand*/
						numWords++;
					}else{	
							reg = isReg((words->word+words->size-1)->str);
							if(reg!=-1){
								RAMWord[0] |= (A_FOUR<<SOAM_SBIT);
								if(label == NULL)/*if the first operand was register*/ 
										RAMWord[1] |= getRegWord(reg,SOR_SBIT);
								else {
										RAMWord[2] = getRegWord(reg,SOR_SBIT);
										numWords++;
									}
							
								}else {
										if(*((words->word+words->size-1)->str)=='*'){
											reg = isReg((words->word+words->size-1)->str+1);
											if(reg!=-1){
												RAMWord[0] |=(A_THREE<<SOAM_SBIT);
												RAMWord[1] |= getRegWord(reg,SOR_SBIT);
											}else{ 	printf("error: wrong in put * before reg\n");
													return false;
													}
										}else{ 
												label=labelExist((words->word+words->size-1)->str);/*check if second operand is label*/
												if(label!=NULL){
													RAMWord[0]|=(A_TWO<<SOAM_SBIT);
													RAMWord[2] = getLabelWord(label);
													numWords++;
													if(label->labelType==EX_LABEL)
														addToExT(label->label,IC+numWords);
												}else {printf("error: unknown operand\n");
													return false;
													}
											}
										}
									}
								}else{	if(reg != -1)
										RAMWord[0]|=(A_FOUR<<SOAM_SBIT);
									else if(label!=NULL)
										RAMWord[0]|=(A_TWO<<SOAM_SBIT);
									} 
						
				break;
			
				case '*':
				reg = isReg((words->word+words->size-1)->str+1);
				if(reg!=-1){
					RAMWord[1] = getRegWord(reg,FOR_SBIT);
					numWords++;
					if(getNextWordInLine(line,words)){
						RAMWord[0]|=(A_THREE<<FOAM_SBIT);
						if(*((words->word+words->size-1)->str)=='#'){
							RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode 0 place in 3 bit*/
							RAMWord[2]=getDirectWord((words->word+words->size-1)->str);/*get the word for # operand*/
							numWords++;
						}else{
								label=labelExist((words->word+words->size-1)->str);/*check if secind operand is label*/
								if(label!=NULL){
									RAMWord[0]|=(A_TWO<<SOAM_SBIT);
									RAMWord[2] = getLabelWord(label);
									numWords++;
									if(label->labelType==EX_LABEL)
										addToExT(label->label,IC+numWords-1);
								}else {	reg = isReg((words->word+words->size-1)->str);
									if(reg!=-1){
										RAMWord[0] |= (A_FOUR<<SOAM_SBIT);
										RAMWord[1] = getRegWord(reg,SOR_SBIT);
									}else if(*((words->word+words->size-1)->str)=='*'){
										reg = isReg((words->word+words->size-1)->str+1);
										if(reg!=-1){
											RAMWord[0] |= (A_THREE<<SOAM_SBIT);
											RAMWord[1] = getRegWord(reg,SOR_SBIT);
										}else{ 	printf("error: wrong in put * before reg\n");
												return false;
												}
									}else{ 	printf("error: unknown operatopr\n");
											return false;
											}
								}
							}
						}else RAMWord[0]|=(A_THREE<<SOAM_SBIT);
					}else{ 	printf("error: wrong in put * before reg\n");
							return false;
							}
				break;
				
				default:
				label=labelExist((words->word+words->size-1)->str);/*check if second operand is label*/
				if(label!=NULL){
					RAMWord[1] = getLabelWord(label);
					numWords++;
					if(label->labelType==EX_LABEL)
							addToExT(label->label,IC+numWords-1);
					if(getNextWordInLine(line,words)){
						RAMWord[0] |= (A_TWO<<FOAM_SBIT);
						numWords++;
						if(*((words->word+words->size-1)->str)=='#'){
							RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode 0 place in 3 bit*/
							RAMWord[2]=getDirectWord((words->word+words->size-1)->str);/*get the word for # operand*/
						}else{
							label=labelExist((words->word+words->size-1)->str);/*check if secind operand is label*/
							if(label!=NULL){
								RAMWord[0] |= (A_TWO<<SOAM_SBIT);
								RAMWord[2] = getLabelWord(label);
								if(label->labelType==EX_LABEL)
									addToExT(label->label,IC+numWords-1);
							}else{	reg = isReg((words->word+words->size-1)->str);
									if(reg!=-1){
										RAMWord[0] |= (A_FOUR<<SOAM_SBIT);
										RAMWord[2] = getRegWord(reg,3);
									}else if(*((words->word+words->size-1)->str)=='*'){
											reg = isReg((words->word+words->size-1)->str+1);
											if(reg!=-1){
												RAMWord[0] |= (A_THREE<<SOAM_SBIT);
												RAMWord[2] = getRegWord(reg,3);
											}else{	printf("error: wrong in put * before reg\n");
													return false;
													}
									}else{	printf("error: unknown operatopr\n");
											return false;
											}
							}
						}
					}else RAMWord[0] |= (A_TWO<<SOAM_SBIT);	
				}else{ 	printf("error: unknown operand\n");
						return false;
						}
						
			}
			
		}
		for(i=0;i<numWords;i++){
			addToMemory(RAMWord[i],&IC);
		}
		return true;
	
}

/*input:char *line 	: 	raw line from file
	lineWords *words: 	an array of words fill with line words until DATA command
output: boolean value	: 	succesfully update RAM and external table
the function update tha RAM and external table accourding DATA line type*/
boolean DataRAMWords(char *line,lineWords *words){
	char *p=NULL;/*pointer for line*/
	if(strcmp((words->word+words->size-1)->str,DATA_CMD)==0){
			while(getNextWordInLine(line,words)){/*while there are stil variables*/
				addToMemory(atoi((words->word+words->size-1)->str),&DC);				
				}
			return true;	
		}else if(strcmp((words->word+words->size-1)->str,STR_CMD)==0){ 
			getNextWordInLine(line,words);
            if((*((words->word+words->size-1)->str) == '\"') && (*((words->word+words->size-1)->str +strlen((words->word+words->size-1)->str)-1) == '\"')){/*check if ther is " un the begining of the string and at the end*/
            	p=(words->word+words->size-1)->str;
            	p++;/*skip " char*/
            	while(p != ((words->word+words->size-1)->str + strlen((words->word+words->size-1)->str) - 1)){
            		addToMemory((int)(*p),&DC);
            		p++;
            		}
            		addToMemory((int)('\0'),&DC);	
				}
				return true;
			}
		return false;
		

}

/*input:lnType lineType:	line type DATA/INST 
	char *line : 		raw line from file
	lineWords *words: 	an array of words fill with line words until DATA/INST command
output: boolean value: 	succesfully update RAM and external table
the function update tha RAM and external table accourding to line type*/
boolean handleRAMWords(lnType lineType,char *line,lineWords *words){
	
	if(lineType==INST)
		return InstRAMWords(line,words);
	else if(lineType==DATA)
		return DataRAMWords(line,words);
	printf("only handle data or instruction RAM words\n");
	return false;
		
}



/*input	: lineWords *words: pointer to line words, the last place in words array is empty
output	: succsess of falier in filling the last place in the word array
the function update the words structer,put the pointer to next word in the added word pointer end
*/
boolean getword(lineWords *words){

	boolean find = false;/*find first word*/
	char *s = NULL,*e=NULL;/*start and end pointers for the requaierd word*/
	char *line=NULL;
	int i=0;/*index*/
	(words->word+words->size-1)->str=NULL;
	if(words->size==1)/*if were finding the first word look for the first end*/
		line = (words->word+words->size-1)->end;
	else line = (words->word+words->size-2)->end;
	while(*line != '\n' && *line != ';' && *line !='\0' && !find){/*until the end of line or find the word*/
		if(*line != '\t' && *line != ';' && *line !=' ' && *line != '\n' && *line !='\0' && *line !=','){/*if were in the start of a word*/
			find=true;
			s=line;
			if(((words->size)==1) || (strcmp((words->word+words->size-2)->str,STR_CMD)!=0)){/*if NOT a string*/
				while(*line != '\t' && *line != ' ' && *line != ';' && *line != '\n' && *line !='\0' && *line != ',') line++;
				
			}else{	if(*line == '\"'){/*if its a string*/
						line = s+strlen(s)-1;
						while(*line != '\"') line--;
						line++;
					}else {printf("error in syntax\n");
							return false;
							}
			}
			e=line;	
		 }else line++;
	}
	if(find){/*creat word*/
		(words->word+words->size-1)->str = (char*)malloc(sizeof(char)*(e-s));
		allcERR((words->word+words->size-1)->str);  
		(words->word+words->size-1)->end = e;
		while(s!=e){
			*((words->word+words->size-1)->str+i) = *s;
			s++;
			i++;
		}
		*((words->word+words->size-1)->str+i) = '\0';
	}	
	return find;
}



/*input	:char *line		: the raw line from file
	 lineWords *words	: structer that contain seperaet words in array words->word
output	:boolean value		: next word exsist or not
the function update the words argument and allocate space for new word.
returns if there is a nex word in line and update words */
boolean getNextWordInLine(char* line,lineWords *words){
	
	if(words->size==0){
        	words->word = malloc(sizeof(lineWord));
        	allcERR(words->word);
        	words->word->end=line;
        }
	words->size++;/*move to next word*/
   	words->word = realloc(words->word,(words->size) *sizeof(lineWord));
   	allcERR(words->word);
	return (getword(words));
}


/*input:	lnType lineType	: type
		char *line	:
		lineWords *words:
handle insruction counter by the type of label
(INST = 'add','move...,DATA =.string "asf",.data 120,2, EXTERN = .extern)*/	
boolean handleIC(lnType lineType,char *line,lineWords *words){
	boolean error = false;
	if(lineType == INST){
		IC++;/*instruction word*/
		if(getNextWordInLine(line,words)){
			IC++;/*extra information for first operand*/
			switch (*((words->word+words->size-1)->str))
				{
					case '#':
					if(getNextWordInLine(line,words))
						IC++;
					break;
					
					case 'r':
					if(isReg((words->word+words->size-1)->str)!=-1){
						if(getNextWordInLine(line,words)){
							if(!((isReg((words->word+words->size-1)->str)!=-1)||(((*((words->word+words->size-1)->str))=='*') && isReg((words->word+words->size-1)->str+1)))){/*check if the seconde word its not a register or a reference to reg (*r3 for exemple)*/
								IC++;
							}
						}
					}else	if(getNextWordInLine(line,words))/*if it starts with r and not a register its a label*/
								IC++;
						
					break;
					
					case '*':
					if(getNextWordInLine(line,words))
						if(!(*((words->word+words->size-1)->str)=='*'))
							IC++;
					break;
					
					default:
					if(getNextWordInLine(line,words))
						IC++;
				}
			}
	}else if(lineType == DATA){
		if(strcmp((words->word+words->size-1)->str,DATA_CMD)==0){
            while(getNextWordInLine(line,words)){/*while there are stil variables*/
            		DC++;
            }
      	}else if(strcmp((words->word+words->size-1)->str,STR_CMD)==0){ 
            	if(getNextWordInLine(line,words)){
		      		if(isStr((words->word+words->size-1)->str))/*check if valid string*/
		      			DC+=strlen((words->word+words->size-1)->str) - 2 + 1;/*ignor " in the start and end and add \0*/
		        }else{	error = true;
		        		 printf("error: no data was enterd\n");
		        	}
            }
	}
	printf("IC:     %d\n",IC);
	return !error;

}

boolean lineFirstPass(char* line, int findex){
   
        lineWords *words = NULL;
        char label[LB_NAME_SIZE];
        boolean notEmpty,error = false;
		words = malloc(sizeof(lineWords));
		allcERR(words);
		words->size=0;     	
        notEmpty = getNextWordInLine(line,words);/*check if the line is not empty, if not save first word to fword*/
        if(notEmpty){/*if the row isnt emty (contain ' ' or tab only)*/
        	if(Islabel(((words->word+words->size-1)->str))){/*if it's label*/	
        		strcpy(label,(words->word+words->size-1)->str);
        		label[strlen(label)-1] = '\0';  /*remove : at the end of label*/
          	  	if(getNextWordInLine(line,words)){/*if word in the place i exist*/
            			if(isCmd((words->word+words->size-1)->str)!=-1){
            				if(addLb(label, INST_LABEL))
            					error = !handleIC(INST,line,words);
            				else error = true;
            			}else{/*not command after label*/
            				if((strcmp((words->word+words->size-1)->str,DATA_CMD)==0) || (strcmp((words->word+words->size-1)->str,STR_CMD)==0)){
            					if(addLb(label, DATA_LABEL))
            						error = !handleIC(DATA,line,words);
            					else error = true;
            				}else if(strcmp(((words->word+words->size-1)->str),EXT_CMD) == 0){/*external line*/
          	  						if(getNextWordInLine(line,words)){
          	  							if(labelExist((words->word+words->size-1)->str)!=NULL){/*if label already exsist*/	
            								error = true;
            								printf("error label exsist\n");
            							}else {
            								error = !addLb((words->word+words->size-1)->str, EX_LABEL);
            								}
            						}
            					}
            			}
            				}else {	error = true;
            						printf("error in syntax\n");
            						}
			}else {
					if(isCmd((words->word+words->size-1)->str)!=-1){/*if its a command*/
            			error = !handleIC(INST,line,words);
            	}else{ 	
            			if((strcmp((words->word+words->size-1)->str,DATA_CMD)==0) || (strcmp((words->word+words->size-1)->str,STR_CMD) == 0)){/*not command after label*/
           						error = !handleIC(DATA,line,words);
            			}else{ 	
            					if(strcmp(((words->word+words->size-1)->str),EXT_CMD) == 0){/*external line*/
          	  						if(getNextWordInLine(line,words)){
          	  							if(labelExist((words->word+words->size-1)->str)!=NULL){/*if label already exsist*/	
            								error = true;
            								printf("error label exsist\n");
            							}else {
            								error = !addLb((words->word+words->size-1)->str, EX_LABEL);
            								}
            						}
            					}else{ if(strcmp((words->word+words->size-1)->str,ENT_CMD) != 0){
            							 	printf("line: %d unknown word: %s line",findex,(words->word+words->size-1)->str); 
            							 error = true; 
            							}
            						}
            			}
            	freeWords(words);
				}         
          	}
	}
	return !error;	
}

/*input	: lineWords *words	: structure of line words
the function free dynamic memory that was allocated*/
void freeWords(lineWords *words){/*free an array of strings*/
	int i=0;
	if(words!=NULL){
		for(i=words->size-1;i>=0;i--)
			free((words->word+i)->str);
		free(words->word);
		free(words);
	}
}



   
