#include "prossescmd.h"


int main(int argc, const char * argv[]){
	
	int i=0;
	if(argc==1)
		printf("no file recived\n");
	else{
		for(i=1;i<argc;i++){
			prossesAsm(argv[i]);
		}
		freeMemory();
	}
	return 1;
}
boolean prossesAsm(const char* fileName)
{
	FILE *fp=NULL;
	IC=100;	/*counter for memory place init 100 place*/
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
		updateDataLables(IC);
		printLbTable();/*print labels table*/
	}
	return !error;
}
boolean secondPass(FILE *fp){

	char buff[LINE_LEN];
	boolean error = false;
	int fIndex=0;
	fseek(fp,0,SEEK_SET);/*return to start of the file*/
	printf("IC=%d\n",IC);
	DC=IC;	/*end of instructions memory from first pass*/
	IC=100;	/*counter for memory place init 100 place*/
	printf("************second pass************\n");
	while (fgets(buff,LINE_LEN, fp) != NULL && !error) 
	{
		fIndex++;
		error = !lineSecondPass(buff,fIndex);	
	}
	printExT();
	printEnT();
	return !error;
}

boolean lineSecondPass(char* line, int findex){
	
	lineWords *words = NULL;
	boolean notEmpty,error=false;
	words = malloc(sizeof(lineWords));     	
    words->size = 0;/*find first word in line*/
    words->str = malloc(sizeof(char*));
        notEmpty = getNextWordInLine(line,words);/*check if the line is not empty, if not save first word to fword*/
        if(notEmpty){/*if the row isnt emty (contain ' ' or tab only)*/
        	if(Islabel(*(words->str))){/*if it's label*/
          	  	if(getNextWordInLine(line,words)){/*if word in the place i exist*/
            			if(isCmd(*(words->str+words->size-1))!=-1){
            				handleRAMWords(INST,line,words);
            			}else{/*not command after label*/
            				if(strcmp(*(words->str+words->size-1),".data")==0 || strcmp(*(words->str+words->size-1),".string")==0){
           						handleRAMWords(DATA,line,words);
            				}else {	error=true;
            						printf("error: unknown word: %s",*(words->str+words->size-1));
            						}
            			}
			}		   
			}else if(isCmd(*(words->str+words->size-1))!=-1){
            			handleRAMWords(INST,line,words);
            	}else if(strcmp(*(words->str+words->size-1),".data")==0 || strcmp(*(words->str+words->size-1),".string")==0){/*not command after label*/
           				handleRAMWords(DATA,line,words);
            	}else if(strcmp(*(words->str+words->size-1),".entry")==0){
            		if(getNextWordInLine(line,words))
            			error = !(addToEnT(*(words->str+words->size-1)));
            		}else if(strcmp(*(words->str+words->size-1),".extern")!=0){/*not external line*/
            			printf("line: %d unknown word: %s line",findex,*(words->str+words->size-1)); 
            			error = true;
            		}
            	freeWords(words);
           }
	return !error;	
}


void handleRAMWords(lnType lineType,char *line,lineWords *words){
	
	unsigned int RAMWord[3]={0,0,0};
	int i=0,numWords=1;/*at least one word*/
	int reg=0;
	labelAd *label=NULL;
	char* p = NULL;
	switch (lineType)
	{
		case INST:
		RAMWord[0]|=(isCmd(*(words->str+words->size-1))<<11);/*add opcode */
		RAMWord[0]|=A_THREE;/*turn on the A bit from ARE*/ 
       	if(getNextWordInLine(line,words)){
      		switch (*(*(words->str+words->size-1)))
			{
				case '#':
				RAMWord[0]|=(A_ONE<<7);/*adress methode 0 place in 7 bit*/
				RAMWord[1]=getDirectWord(*(words->str+words->size-1));/*get the word for # operand*/ 
				numWords++;
				if(getNextWordInLine(line,words)){
					numWords++;
					label=labelExist(*(words->str+words->size-1));/*check if second operand is label*/
					if(label!=NULL){
						RAMWord[0]|=(A_TWO<<3);
						RAMWord[2]=getLabelWord(label);
						if(label->labelType==EX_LABEL)
							addToExT(label->label,IC+2);
					}else {	reg = isReg(*(words->str+words->size-1));
							if(reg!=-1){
								RAMWord[0]|=(A_FOUR<<3);
								RAMWord[2] = getRegWord(reg,3);
							}else if(**(words->str+words->size-1)=='*'){
									reg = isReg(*(words->str+words->size-1)+1);
									if(reg!=-1){
											RAMWord[0]|=(A_THREE<<3);
											RAMWord[2] = getRegWord(reg,3);
									}else printf("error: wrong in put * before reg\n");
							}else printf("error: unknown operator\n");
						}
				}
				break;
				
				case 'r':
				numWords++;
				reg = isReg(*(words->str+words->size-1));
				if(reg!=-1){
					RAMWord[0]|=(A_FOUR<<7);
					RAMWord[1] = getRegWord(reg,6);	
				}else {
					label=labelExist(*(words->str+words->size-1));/*check if secind operand is label*/
					if(label!=NULL){
						RAMWord[0]|=(A_TWO<<7);
						RAMWord[1]=getLabelWord(label);
						if(label->labelType==EX_LABEL)
							addToExT(label->label,IC+1);	
					}else printf("error: unknown operand\n");
				}			
				if(getNextWordInLine(line,words)){
					if(**(words->str+words->size-1)=='#'){
						RAMWord[0]|=(A_ONE<<3);/*adress methode 0 place in 3 bit*/
						RAMWord[2]=getDirectWord(*(words->str+words->size-1));/*get the word for # operand*/
						numWords++;
					}else{
							label=labelExist(*(words->str+words->size-1));/*check if second operand is label*/
							if(label!=NULL){
								RAMWord[0]|=(A_TWO<<3);
								RAMWord[2] = getLabelWord(label);
								if(label->labelType==EX_LABEL)
									addToExT(label->label,IC+2);
								numWords++;
							}else {	reg = isReg(*(words->str+words->size-1));
								if(reg!=-1){
									RAMWord[0]|=(A_FOUR<<3);
									RAMWord[1] |= getRegWord(reg,3);
								}else if(**(words->str+words->size-1)=='*'){
										reg = isReg(*(words->str+words->size-1)+1);
										if(reg!=-1){
											RAMWord[0]|=(A_THREE<<3);
											RAMWord[1] |= getRegWord(reg,3);
										}else printf("error: wrong in put * before reg\n");
									}else printf("error: unknown operatopr\n");
									
							
								}
							}
						
				break;
			
				case '*':
				reg = isReg(*(words->str+words->size-1)+1);
				if(reg!=-1){
					RAMWord[0]|=(A_THREE<<7);
					RAMWord[1] = getRegWord(reg,6);
					numWords++;
					if(getNextWordInLine(line,words)){
						numWords++;
						if(**(words->str+words->size-1)=='#'){
							RAMWord[0]|=(A_ONE<<3);/*adress methode 0 place in 3 bit*/
							RAMWord[2]=getDirectWord(*(words->str+words->size-1));/*get the word for # operand*/
							numWords++;
						}else{
								label=labelExist(*(words->str+words->size-1));/*check if secind operand is label*/
								if(label!=NULL){
									RAMWord[0]|=(A_TWO<<3);
									RAMWord[2] = getLabelWord(label);
									if(label->labelType==EX_LABEL)
										addToExT(label->label,IC+2);
								}else {	reg = isReg(*(words->str+words->size-1));
									if(reg!=-1){
										RAMWord[0] |= (A_FOUR<<3);
										RAMWord[1] = getRegWord(reg,3);
									}else if(**(words->str+words->size-1)=='*'){
										reg = isReg(*(words->str+words->size-1)+1);
										if(reg!=-1){
											RAMWord[0] |= (A_THREE<<3);
											RAMWord[1] = getRegWord(reg,3);
										}else printf("error: wrong in put * before reg\n");
									}else printf("error: unknown operatopr\n");
								}
							}
						}
					}else printf("error: wrong in put * before reg\n");
				break;
				
				default:
				label=labelExist(*(words->str+words->size-1));/*check if second operand is label*/
				if(label!=NULL){
					RAMWord[0] |= (A_TWO<<7);
					RAMWord[1] = getLabelWord(label);
					if(label->labelType==EX_LABEL)
							addToExT(label->label,IC+1);
					numWords++;
					if(getNextWordInLine(line,words)){
						numWords++;
						if(**(words->str+words->size-1)=='#'){
							RAMWord[0]|=(A_ONE<<3);/*adress methode 0 place in 3 bit*/
							RAMWord[2]=getDirectWord(*(words->str+words->size-1));/*get the word for # operand*/
						}else{
							label=labelExist(*(words->str+words->size-1));/*check if secind operand is label*/
							if(label!=NULL){
								RAMWord[0] |= (A_TWO<<3);
								RAMWord[2] = getLabelWord(label);
								if(label->labelType==EX_LABEL)
									addToExT(label->label,IC+2);
							}else{	reg = isReg(*(words->str+words->size-1));
									if(reg!=-1){
										RAMWord[0] |= (A_FOUR<<3);
										RAMWord[2] = getRegWord(reg,3);
									}else if(**(words->str+words->size-1)=='*'){
											reg = isReg(*(words->str+words->size-1)+1);
											if(reg!=-1){
												RAMWord[0] |= (A_THREE<<3);
												RAMWord[2] = getRegWord(reg,3);
											}else printf("error: wrong in put * before reg\n");
									}else printf("error: unknown operatopr\n");
							}
						}
					}	
				}else printf("error: unknown operand\n");
				
				}		
			}
			
		}
			for(i=0;i<numWords;i++){
				addToMemory(RAMWord[i],&IC);
				}
		break;
				
		case DATA:
		if(strcmp(*(words->str+words->size-1),".data")==0){
			while(getNextWordInLine(line,words)){/*while there are stil variables*/
				addToMemory(atoi(*(words->str+words->size-1)),&DC);				
				}	
		}else if(strcmp(*(words->str+words->size-1),".string")==0){ 
			getNextWordInLine(line,words);
            if((**(words->str+words->size-1) == '\"') && (*(*(words->str+words->size-1) +strlen(*(words->str+words->size-1))-1) == '\"')){/*check if tther is " un the begining of the string and at the end*/
            	p=*(words->str+words->size-1);
            	p++;/*skip " char*/
            	while(*p!=(*(words->str+words->size-1))[strlen(*(words->str+words->size-1)-1)]){
            		printf("DC: %d\n",DC);
            		addToMemory((int)(*p),&DC);
            		p++;
            		}
            		addToMemory((int)('\0'),&DC);	
				}
			}
		
		break;
	}
		
}




boolean getword(char* line,char **word, int num){

	boolean find = false;/*find first word*/
	char *s = NULL,*e=NULL;/*start and end pointers for the requaierd word*/
	int i=0;/*index*/
	if(word == NULL)
	printf("null\n");
	*word=NULL;
	while(*line != '\n' && *line !='\0' && !find){/*until the end of line or find the word*/
		if(*line != '\t' && *line !=' ' && *line != '\n' && *line !='\0' && *line !=','){/*if were in the start of a word*/
			if(num == 0)
				find = true;
			else while(*line != '\t' && *line !=' ' && *line != '\n' && *line !='\0' && *line != ',')line++;/*jump after the word*/
			num--;
		}
		if(find){/*if were in the right word*/
			s=line;
			while(*line != '\t' && *line != ' ' && *line != '\n' && *line !='\0' && *line != ',') line++;
			e=line;	
		}else line++;/*if didnt find the right word move to next char*/	
	}
	if(find){/*creat word*/
		*word = (char*)malloc(sizeof(char)*(e-s));
		while(s!=e){
			*(*word+i) = *s;
			s++;
			i++;
		}
		*(*word+i) = '\0';
		printf("%s \n",*word);
	}	
	return find;
}

boolean getNextWordInLine(char* line,lineWords *words){
	words->size++;/*move to next word*/
    words->str = realloc(words->str,(words->size) *sizeof(char*));;
	return (getword(line,words->str+words->size-1,words->size-1));
}
void handleIC(lnType lineType,char *line,lineWords *words){/*handle internale counter by the type of label
(INST = 'add','move...,DATA =.string "asf",.data 120,2, EXTERN = .extern)*/	
	if(lineType == INST){
		if(getNextWordInLine(line,words)){
			switch (*(*(words->str+words->size-1)))
				{
					case '#':
					if(getNextWordInLine(line,words))
						IC+=3;
					else IC+=2;
					break;
					
					case 'r':
					if(isReg(*(words->str+words->size-1))!=-1){
						if(getNextWordInLine(line,words)){
							if((isReg(*(words->str+words->size-1))!=-1)||(((**(words->str+words->size-1))=='*') && isReg(*(words->str+words->size-1)+1))){
								IC+=2;
							}else IC+=3;
						}else IC+=2;
					}else	if(getNextWordInLine(line,words))/*is label*/
								IC+=3;
							else IC+=2;
					break;
					
					case '*':
					if(getNextWordInLine(line,words))
						if(**(words->str+words->size-1)=='*')
							IC+=2;
						else IC+=3;
					else IC+=2;
					break;
					
					default:
					if(getNextWordInLine(line,words))
						IC+=3;
					else IC+=2;
				}
			}else IC++;
	}else if(lineType == DATA){
		if(strcmp(*(words->str+words->size-1),".data")==0){
            while(getNextWordInLine(line,words)){/*while there are stil variables*/
            		DC++;
  					getNextWordInLine(line,words);
            }
      	}else if(strcmp(*(words->str+words->size-1),".string")==0){ 
            	if(getNextWordInLine(line,words)){
		      		if(isString(*(words->str+words->size-1)))/*check if valid string*/
		      			DC+=strlen(*(words->str+words->size-1)) - 2 + 1;/*ignor " in the start and end and add \0*/
		        }else printf("error: no data was enterd\n");
            }
	}
	printf("IC:     %d\n",IC);

}

boolean lineFirstPass(char* line, int findex){
   
        lineWords *words = NULL;
        char *label=NULL;
        boolean notEmpty;
		words = malloc(sizeof(lineWords));     	
        words->size = 0;/*find first word in line*/
        words->str = malloc(sizeof(char*));
        notEmpty = getNextWordInLine(line,words);/*check if the line is not empty, if not save first word to fword*/
        if(notEmpty){/*if the row isnt emty (contain ' ' or tab only)*/
        	if(Islabel((*(words->str)))){/*if it's label*/	
        		label = *(words->str);
        		label[strlen(label)-1] = '\0';  /*remove : at the end of label*/
          	  	if(getNextWordInLine(line,words)){/*if word in the place i exist*/
            			if(isCmd(*(words->str+words->size-1))!=-1){
            				addLb(label, INST_LABEL);
            				handleIC(INST,line,words);
            			}else{/*not command after label*/
            				if(strcmp(*(words->str+words->size-1),".data")==0 || strcmp(*(words->str+words->size-1),".string")==0){
            					addLb(label, DATA_LABEL);
            					handleIC(DATA,line,words);
            				}
            			}
            				}else printf("error in syntax\n");
			}else {if(isCmd(*(words->str+words->size-1))!=-1){/*if its a command*/
            		handleIC(INST,line,words);
            	}else{ 	
            			if((strcmp((*(words->str+words->size-1)),".data")==0) || (strcmp((*(words->str+words->size-1)),".string") == 0)){/*not command after label*/
           						handleIC(DATA,line,words);
            			}else{ 	
            					if(strcmp((*(words->str+words->size-1)),".extern") == 0){/*external line*/
            					words->size++;/*next word index*/
          	  					words = realloc(words,(words->size)*sizeof(char*));
          	  					if(getword(line,words->str+words->size-1,words->size-1)){
          	  						if(labelExist(*(words->str+words->size-1))!=NULL){/*if it's label*/	
            							printf("error label exsist\n");
            						}else {
            							addLb(*(words->str+words->size-1), EX_LABEL);
            							}
            					}
            				}else{if(strcmp(*(words->str+words->size-1),".entry") != 0)
            							 printf("line: %d unknown word: %s line",findex,*(words->str+words->size-1));  
            						}
            					}
            		}
            	freeWords(words);
			}         
          }
	return true;	
}

void freeWords(lineWords *words){/*free an array of strings*/
	int i=0;
	if(words!=NULL){
		for(i=words->size-1;i>=0;i--){
			free(*(words->str+i));
		}
		free(words);
	}

}



   
