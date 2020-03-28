#include "FirstPassUtils.h"

/*input	:char* line: raw line
	 	 int findex: index of line in the file
output	:boolean value of success or failier of first pass */
boolean lineFirstPass(char* line, int findex){
   
        lineWords *words = NULL;/*lineWords pointer to structure*/
        char label[LB_NAME_SIZE];/*label name*/
        boolean notEmpty,error = false;
        labelAd *labelp = NULL;/*pointer to lable*/
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
            								if(labelp->labelType!=EX_LABEL){/*if the lable type is not external*/
            									error = true;
            									printf("error diffrent decleration of label : %s \n",(words->word+words->size-1)->str);
            								}
            							}else {
            								error = !addLb((words->word+words->size-1)->str, EX_LABEL);/*add external label*/
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
          	  							labelp = labelExist((words->word+words->size-1)->str);
          	  							if(labelp!=NULL){/*if label already exsist*/	
            								if(labelp->labelType!= EX_LABEL){/*if the lable type is not external*/
            									error = true;
            									printf("error diffrent decleration of label : %s \n",(words->word+words->size-1)->str);
            								}
            							}else {
            								error = !addLb((words->word+words->size-1)->str, EX_LABEL);
            								}
            						}
            					}else{ if(strcmp((words->word+words->size-1)->str,ENT_CMD) != 0){
            							 	printf("unknown word: %s \n",(words->word+words->size-1)->str); 
            							 error = true; 
            							}
            						}
            			}
				}         
          	}
	}
	freeWords(words);
	if(error)
		printf("Row %d: error in row. \n",findex);
            	
	return !error;	
}



/*input:	lnType lineType	: type
		char *line	: raw line from file
		lineWords *words: lineWords contain the words in the line in words->word array
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
					if((isReg((words->word+words->size-1)->str)!=-1)){
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
						if((*((words->word+words->size-1)->str) != '*') && (isReg((words->word+words->size-1)->str) ==-1))/*check if second operand is register*/
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
	/*printf("IC:     %d\n",IC); check the IC counetr in first pass*/
	return !error;

}




   
