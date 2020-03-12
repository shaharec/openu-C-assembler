#include "DataStruct.h"
boolean prossesAsm(char* fileName);
boolean firstPass(FILE *fp);
boolean secondPass(FILE *fp);
boolean getword(char* line,char *word, int num);
boolean getLineInfo(char* line, int findex);
void handleIC(lnType lineType,char *line,char **words, int i);
void freeWords(char **words);


boolean prossesAsm(char* fileName)
{
	FILE *fp=NULL;
	IC=100;	/*counter for memory place init 100 place*/
	DC=0;	/*counter for label place init 0 place*/
	fp = fopen(fileName, "r");
    	if (fp == NULL){
        	printf("no file found\n");
        	return false;}
        else{
		if(first_pass(fp))
			if(second_pass(fp))
				return true;
		return false;
	printf("finish");
	}
}
boolean firstPass(FILE *fp){

	char buff[LINE_LEN];
	boolean error = false;
	int fIndex=0;
	lineInfo *line = NULL;
	IC=100;	/*counter for memory place init 100 place*/
	DC=0;	/*counter for label place init 100 place*/
	while (fgets(buff,LINE_LEN, fp) != NULL && !error) 
	{
		fIndex++;
		error = getLineInfo(buff,fIndex);	
	}
	return !error;
}
boolean secondPass(FILE *fp){
	return true;
}


boolean getword(char* line,char *word, int num){

	boolean find = false;/*find first word*/
	char *s = NULL,*e=NULL;/*start and end pointers for the requaierd word*/
	int i=0;/*index*/
	while(*line != '\n' && *line !='\0' && !find){/*until the end of line or find the word*/
		if(*line != '\t' && *line !=' ' && *line != '\n' && *line !='\0' && *line !=','){/*if were in the start of a word*/
			if(num == 0)
				find = true;
			else while(*line != '\t' && *line !=' '&& *line != '\n' && *line !='\0' && *line !=',')line++;/*jump after the word*/
			num--;
		}
		if(find){/*if were in the right word*/
			s=line;
			while(*line != '\t' && *line !=' '&& *line != '\n' && *line !='\0'&& *line !=',') line++;
			e=line;	
		}else line++;/*if didnt find the right word move to next char*/	
	}
	if(find){/*creat word*/
		word = (char*)malloc(sizeof(char)*(e-s));
		while(s!=e){
			*(word+i) = *s;
			s++;
			i++;
		}
		*(word+i) = '\0';
	}	
	return find;
}

void handleIC(lnType lineType,char *line,char **words, int i){/*handle internale counter by the type of label
(INST = 'add','move...,DATA =.string "asf",.data 120,2, EXTERN = .extern)*/
	i++;/*move to operand/data/extern word*/
        words = realloc(words,(i+1) *sizeof(char*));;
	if(lineType == INST){
	switch (*(*(words+i)))
		{
			case '#':
			i++;
			words = realloc(words,(i+1) *sizeof(char*));;
			if(getword(line,*(words+i),i))
				IC+=3;
			else IC+=2;
			break;
			
			case 'r':
			i++;
			words = realloc(words,(i+1) *sizeof(char*));;
			if(getword(line,*(words+i),i))
				if(**(words+i)=='r')
					IC+=2;
				else IC+=3;
			else IC+=2;
			
			case '*':
			i++;
			words = realloc(words,(i+1) *sizeof(char*));;
			if(getword(line,*(words+i),i))
				if(**(words+i)=='*')
					IC+=2;
				else IC+=3;
			else IC+=2;
			
			default:
			i++;
			words = realloc(words,(i+1) *sizeof(char*));
			if(getword(line,*(words+i),i))
				IC+=3;
			else IC+=2;
		}
		}else if(lineType == DATA){
			if(strcmp(*(words+i),".data")==0){
            						while(*(words+i)){/*while there are stil variables*/
            							i++;
            							words = realloc(words,(i+1) *sizeof(char*));
            							if(getword(line,*(words+i),i))
            								IC++;	
            						}
            		}else if(strcmp(*(words+i),".string")==0){ 
            			i++;
            			words = realloc(words,(i+1) *sizeof(char*));
            			if(getword(line,*(words+i),i))
            				if(**(words+i) == '"' && *(*(words+i) +strlen(*(words+i))) == '"')
            					IC+=strlen(*(words+i)) - 2 + 1;/*ignor the " in the begining and end of the string but add /0 in the 														end*/	
            			}
		}else if(lineType == EX){
			IC++;		
		}

}

boolean getLineInfo(char* line, int findex){
   
        char **words = NULL;
        char *label=NULL;
        boolean notEmpty;
        int i=0;
         words = realloc(words,(i+1) *sizeof(char*));
        notEmpty = getword(line,*words,i);/*check if the line is not empty, if not save first word to fword*/
        if(notEmpty){/*if the row isnt emty (contain ' ' or tab only)*/
        	if(Islabel(*words)){/*if it's label*/
        		label = *words;
        		label[strlen(label)-1] = '\0';  /*remove : at the end of label*/
         	 	i++;/*next word index*/
          	  	words = realloc(words,(i+1) *sizeof(char*));
          	  	if(getword(line,*(words+i),i)){/*if word in the place i exist*/
            			if(isCmd(*(words+i))){
            				addLb(label, INST_LABEL);
            				handleIC(INST,line,words,i);
            			}else{/*not command after label*/
            				printf("data command");
            				if(strcmp(*(words+i),".data")==0 || strcmp(*(words+i),".string")==0){
            					addLb(label, DATA_LABEL);
           					handleIC(DATA,line,words,i);
            				}else if(strcmp(*(words+i),".extern")==0){
            					addLb(label, EX_LABEL);
            					handleIC(EX,line,words,i);
            				}
            			}
			}		   
		}else if(isCmd(*(words+i))){
            				addLb(label, INST_LABEL);
            				handleIC(INST,line,words,i);
            		}else if(strcmp(*(words+i),".data")==0 || strcmp(*(words+i),".string")==0){/*not command after label*/
            				addLb(label,DATA);
           				handleIC(DATA_LABEL,line,words,i);
            		}else if(strcmp(*(words+i),".extern")==0){/*external line*/
            				addLb(label, EX);
            				handleIC(EX_LABEL,line,words,i);
            			}
            	freeWords(words);
           }
	return true;	
}

void freeWords(char **words){
	size_t n = sizeof(words)/sizeof(*words);
	int i=0;
	if(words!=NULL){
		for(i=0;i<n;i++){
			free(*(words+i));
		}
		free(words);
	}

}



   
