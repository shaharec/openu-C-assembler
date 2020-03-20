#include "prossescmd.h"


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
	printf("finish");
	return true;
	}
}
boolean firstPass(FILE *fp){

	char buff[LINE_LEN];
	boolean error = false;
	int fIndex=0;
	IC=100;	/*counter for memory place init 100 place*/
	DC=0;	/*counter for label place init 100 place*/
	printf("************first pass************\n");
	while (fgets(buff,LINE_LEN, fp) != NULL && !error) 
	{
		printf("%s",buff);
		fIndex++;
		error = !lineFirstPass(buff,fIndex);	
	}
	if(!error){
		updateDataLabels(IC);
		printLbTable();/*print labels table*/
	}
	return !error;
}
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
	updateRAMCounters();
	printExT();
	printEnT();
	return !error;
}

boolean lineSecondPass(char* line, int findex){
	
	lineWords *words = NULL;
	boolean notEmpty,error=false;
	words = malloc(sizeof(lineWords));     	
    words->size = 0;/*find first word in line*/
        notEmpty = getNextWordInLine(line,words);/*check if the line is not empty, if not save first word to fword*/
        if(notEmpty){/*if the row isnt emty (contain ' ' or tab only)*/
        	if(Islabel(words->word->str)){/*if it's label*/
          	  	if(getNextWordInLine(line,words)){/*if word in the place i exist*/
            			if(isCmd((words->word+words->size-1)->str)!=-1){
            				handleRAMWords(INST,line,words);
            			}else{/*not command after label*/
            				if(strcmp((words->word+words->size-1)->str,".data")==0 || strcmp((words->word+words->size-1)->str,".string")==0){
           						handleRAMWords(DATA,line,words);
            				}else	if(strcmp((words->word+words->size-1)->str,".entry")==0){
            							if(getNextWordInLine(line,words))
            								error = !(addToEnT((words->word+words->size-1)->str));
            					}else {	error=true;
            						printf("error: unknown word: %s",(words->word+words->size-1)->str);
            						}
            			}
			}		   
			}else if(isCmd((words->word+words->size-1)->str)!=-1){
            			handleRAMWords(INST,line,words);
            	}else if(strcmp((words->word+words->size-1)->str,".data")==0 || strcmp((words->word+words->size-1)->str,".string")==0){/*not command after label*/
           				handleRAMWords(DATA,line,words);
            	}else if(strcmp((words->word+words->size-1)->str,".entry")==0){
            		if(getNextWordInLine(line,words))
            			error = !(addToEnT((words->word+words->size-1)->str));
            		}else if(strcmp((words->word+words->size-1)->str,".extern")!=0){/*not external line*/
            			printf("line: %d unknown word: %s line",findex,(words->word+words->size-1)->str); 
            			error = true;
            		}
            	freeWords(words);
           }
	return !error;	
}

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
									}else{ 	printf("error: wrong in put * before reg\n");
											return false;
										}
							}else {	printf("error: unknown operator\n");
									return false;
									}
						}
				}else RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode 0 place in second operand bit*/
				break;
				
				case 'r':
				numWords++;
				reg = isReg((words->word+words->size-1)->str);
				if(reg!=-1){
					RAMWord[1] = getRegWord(reg,FOR_SBIT);	
				}else {
					label=labelExist((words->word+words->size-1)->str);/*check if secind operand is label*/
					if(label!=NULL){
						RAMWord[0]|=(A_TWO<<SOAM_SBIT);
						RAMWord[1]=getLabelWord(label);
						if(label->labelType==EX_LABEL)
							addToExT(label->label,IC+numWords-1);	
					}else {	printf("error: unknown operand\n");
							return false;
							}
				}			
				if(getNextWordInLine(line,words)){
					if(*((words->word+words->size-1)->str)=='#'){
						RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode 0 place in 3 bit*/
						RAMWord[2]=getDirectWord((words->word+words->size-1)->str);/*get the word for # operand*/
						numWords++;
					}else{	reg = isReg((words->word+words->size-1)->str);
							if(reg!=-1){
								RAMWord[0] |= (A_FOUR<<SOAM_SBIT);
								if(label == NULL)/*if the first operand was register*/ 
										RAMWord[1] = getRegWord(reg,SOR_SBIT);
								else {
										RAMWord[2] = getRegWord(reg,SOR_SBIT);
										numWords++;
									}
							
								}else {
										if(*((words->word+words->size-1)->str)=='*'){
											reg = isReg((words->word+words->size-1)->str+1);
											if(reg!=-1){
												RAMWord[0]|=(A_THREE<<SOAM_SBIT);
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
												}
											}
										}
									}
								}else{	if(reg != -1)
										RAMWord[0]|=(A_FOUR<<FOAM_SBIT);
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

boolean DataRAMWords(char *line,lineWords *words){
	char *p=NULL;
	if(strcmp((words->word+words->size-1)->str,".data")==0){
			while(getNextWordInLine(line,words)){/*while there are stil variables*/
				addToMemory(atoi((words->word+words->size-1)->str),&DC);				
				}
			return true;	
		}else if(strcmp((words->word+words->size-1)->str,".string")==0){ 
			getNextWordInLine(line,words);
            if((*((words->word+words->size-1)->str) == '\"') && (*((words->word+words->size-1)->str +strlen((words->word+words->size-1)->str)-1) == '\"')){/*check if tther is " un the begining of the string and at the end*/
            	p=(words->word+words->size-1)->str;
            	p++;/*skip " char*/
            	while(*p!=(((words->word+words->size-1)->str)[strlen((words->word+words->size-1)->str)-1])){
            		printf("DC: %d\n",DC);
            		addToMemory((int)(*p),&DC);
            		p++;
            		}
            		addToMemory((int)('\0'),&DC);	
				}
				return true;
			}
		return false;
		

}
boolean handleRAMWords(lnType lineType,char *line,lineWords *words){
	
	if(lineType==INST)
		return InstRAMWords(line,words);
	else if(lineType==DATA)
		return DataRAMWords(line,words);
	printf("only handle data or istruction ram words\n");
	return false;
		
}




boolean getword(lineWords *words){

	boolean find = false;/*find first word*/
	char *s = NULL,*e=NULL;/*start and end pointers for the requaierd word*/
	char *line=NULL;
	int i=0;/*index*/
	(words->word+words->size-1)->str=NULL;
	if(words->size==1)
		line = (words->word+words->size-1)->end;
	else line = (words->word+words->size-2)->end;
	while(*line != '\n' && *line !='\0' && !find){/*until the end of line or find the word*/
		if(*line != '\t' && *line !=' ' && *line != '\n' && *line !='\0' && *line !=','){/*if were in the start of a word*/
			find=true;
			s=line;
			if(((words->size)==1) || (strcmp((words->word+words->size-2)->str,".string")!=0)){/*if NOT a string*/
				while(*line != '\t' && *line != ' ' && *line != '\n' && *line !='\0' && *line != ',') line++;
			}else{	if(*line == '\"'){/*if its a string*/
						line++;
						while(*line != '\"' && *line+1 != '\n' && *line+1 !='\0') line++;
						line++;/*go to the " char*/
					}else {printf("error in syntax\n");
							return false;
							}
			}
			e=line;	
		 }else line++;
	}
	if(find){/*creat word*/
		(words->word+words->size-1)->str = (char*)malloc(sizeof(char)*(e-s));
		(words->word+words->size-1)->end = e;
		while(s!=e){
			*((words->word+words->size-1)->str+i) = *s;
			s++;
			i++;
		}
		*((words->word+words->size-1)->str+i) = '\0';
		printf("%s\n",(words->word+words->size-1)->str);
	}	
	return find;
}



boolean getNextWordInLine(char* line,lineWords *words){
	if(words->size==0){
        words->word = malloc(sizeof(lineWord));
        words->word->end=line;
        }
	words->size++;/*move to next word*/
    words->word = realloc(words->word,(words->size) *sizeof(lineWord));
	return (getword(words));
}

void handleIC(lnType lineType,char *line,lineWords *words){/*handle internale counter by the type of label
(INST = 'add','move...,DATA =.string "asf",.data 120,2, EXTERN = .extern)*/	
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
							if(!((isReg((words->word+words->size-1)->str)!=-1)||(((*((words->word+words->size-1)->str))=='*') && isReg((words->word+words->size-1)->str+1)))){
								IC++;;
							}
						}
					}else	if(getNextWordInLine(line,words))/*is label*/
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
		if(strcmp((words->word+words->size-1)->str,".data")==0){
            while(getNextWordInLine(line,words)){/*while there are stil variables*/
            		DC++;
            }
      	}else if(strcmp((words->word+words->size-1)->str,".string")==0){ 
            	if(getNextWordInLine(line,words)){
		      		if(isString((words->word+words->size-1)->str))/*check if valid string*/
		      			DC+=strlen((words->word+words->size-1)->str) - 2 + 1;/*ignor " in the start and end and add \0*/
		        }else printf("error: no data was enterd\n");
            }
	}
	printf("IC:     %d\n",IC);

}

boolean lineFirstPass(char* line, int findex){
   
        lineWords *words = NULL;
        char label[LB_NAME_SIZE];
        boolean notEmpty;
		words = malloc(sizeof(lineWords));
		words->size=0;     	
        notEmpty = getNextWordInLine(line,words);/*check if the line is not empty, if not save first word to fword*/
        if(notEmpty){/*if the row isnt emty (contain ' ' or tab only)*/
        	if(Islabel(((words->word+words->size-1)->str))){/*if it's label*/	
        		strcpy(label,(words->word+words->size-1)->str);
        		label[strlen(label)-1] = '\0';  /*remove : at the end of label*/
          	  	if(getNextWordInLine(line,words)){/*if word in the place i exist*/
            			if(isCmd((words->word+words->size-1)->str)!=-1){
            				addLb(label, INST_LABEL);
            				handleIC(INST,line,words);
            			}else{/*not command after label*/
            				if((strcmp((words->word+words->size-1)->str,".data")==0) || (strcmp((words->word+words->size-1)->str,".string")==0)){
            					addLb(label, DATA_LABEL);
            					handleIC(DATA,line,words);
            				}
            			}
            				}else printf("error in syntax\n");
			}else {
					if(isCmd((words->word+words->size-1)->str)!=-1){/*if its a command*/
            			handleIC(INST,line,words);
            	}else{ 	
            			if((strcmp((words->word+words->size-1)->str,".data")==0) || (strcmp((words->word+words->size-1)->str,".string") == 0)){/*not command after label*/
           						handleIC(DATA,line,words);
            			}else{ 	
            					if(strcmp(((words->word+words->size-1)->str),".extern") == 0){/*external line*/
          	  						if(getNextWordInLine(line,words)){
          	  							if(labelExist((words->word+words->size-1)->str)!=NULL){/*if it's label*/	
            								printf("error label exsist\n");
            							}else {
            								addLb((words->word+words->size-1)->str, EX_LABEL);
            								}
            						}
            					}else{ if(strcmp((words->word+words->size-1)->str,".entry") != 0)
            							 printf("line: %d unknown word: %s line",findex,(words->word+words->size-1)->str);  
            						}
            			}
            	freeWords(words);
				}         
          	}
	}
	return true;	
}

void freeWords(lineWords *words){/*free an array of strings*/
	int i=0;
	if(words!=NULL){
		for(i=words->size-1;i>=0;i--)
			free((words->word+i)->str);
		free(words->word);
		free(words);
	}
}



   
