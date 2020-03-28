#include "LineUtils.h"


/*input	: lineWords *words: pointer to line words, the last place in words array is empty
output	: succsess of falier in filling the last place in the word array
the function update the words structer,put the pointer to next word in the added word pointer end
*/
boolean getword(lineWords *words){

	boolean find = false;/*find first word*/
	char *s = NULL,*e=NULL;/*start and end pointers for the requaierd word*/
	char *line=NULL;/*pointer to char in line*/
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



/*input	:char *line			: the raw line from file
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


/*input : char *str
output	: int value : value of a information word of label value word
the function get label and returns the RAM information word */
int isReg(char *str){
	
	int i=0;
	for(i=0;i<REG_NUM;i++){
		if(strcmp(reg[i],str) == 0)
			return i;
		}
	return -1;
}


   
