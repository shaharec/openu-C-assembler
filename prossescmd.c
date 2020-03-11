#include "DataStruct.h"
boolean prossesAsm(char* fileName);
boolean firstPass(FILE *fp);
boolean secondPass(FILE *fp);
boolean getword(char* line,char *word, int num);
boolean getLineInfo(char* line, int findex);


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



boolean getLineInfo(char* line, int findex){
   
        char **words = NULL;
        char *lable=NULL;
        boolean notEmpty;
        int i=0;
         words = realloc(words,(i+1) *sizeof(char*));
        notEmpty = getword(line,*words,i);/*check if the line is not empty, if not save first word to fword*/
        if(notEmpty){/*if the row isnt emty (contain ' ' or tab only)*/
        	if(Islabel(*words)){/*if it's label*/
        	    lable = *words;
        	    lable[strlen(lable)-1] = '\0';  /*remove : at the end of label*/
         	   i++;/*next word index*/
          	  words = realloc(words,(i+1) *sizeof(char*));
          	  if(getword(line,*(words+i),i)){/*if word in the place i exist*/
            		if(isCmd(*(words+i))){
            			addLb(lable, INST_LABEL);
            			i++;/*move to operand word*/
            			words = realloc(words,(i+1) *sizeof(char*));;
            			if(getword(line,*(words+i),i))
            				switch (*(*(words+i)))
            				{
            					case '#':
            					i++;
            					words = realloc(words,(i+1) *sizeof(char*));;
            					if(getword(line,*(words+i),i))
            						IC+=2;
            					else IC+=1;
            					break;
            					
            					case 'r':
            					i++;
            					words = realloc(words,(i+1) *sizeof(char*));;
            					if(getword(line,*(words+i),i))
            						if(**(words+i)=='r')
            							IC+=1;
            						else IC+=2;
            					else IC+=1;
            					
            					case '*':
            					i++;
            					words = realloc(words,(i+1) *sizeof(char*));;
            					if(getword(line,*(words+i),i))
            						if(**(words+i)=='*')
            							IC+=1;
            						else IC+=2;
            					else IC+=1;
            					
            					default:
            					i++;
            					words = realloc(words,(i+1) *sizeof(char*));
            					if(getword(line,*(words+i),i))
            						IC+=2;
            					else IC+=1;
            				}
            			}else{/*not command after label*/
            				printf("data command");
            			}
			}		   
		} 
		return true;	
	}
}


   
