#include "FirstPassUtils.h"

/*input	:char* line: raw line
	 	 		 int findex: index of line in the file
output	:boolean value of success or failier of first pass
the function preform the first pass logic.
count RAM words and fill the label table */
boolean lineFirstPass(char* line, int findex){
   
        lineWords *words = NULL;		/*lineWords pointer to structure*/
        char label[LB_NAME_SIZE];		/*label name*/
        boolean notEmpty,error = false;		/*boolean value for empty row and error*/
        labelAd *labelp = NULL;			/*pointer to lable*/
	words = malloc(sizeof(lineWords));	
	allcERR(words);
	words->size=0;     	
        notEmpty = getNextWordInLine(line,words);/*check if the line is not empty, if not save first word to fword*/
        if(notEmpty){/*if the row isnt emty (contain ' ' or tab only)*/
        	if(Islabel(((words->word+words->size-1)->str))){/*if it's label*/	
        		strcpy(label,(words->word+words->size-1)->str);
        		label[strlen(label)-1] = '\0';  /*remove : at the end of label*/
          	  	if(getNextWordInLine(line,words)){/*if word in the place i exist*/
            			if(isCmd((words->word+words->size-1)->str)!=-1){/*check if command*/
            				if(addLb(label, INST_LABEL))/*add instruction label*/
            					error = !handleIC(INST,line,words);/*update instruction counter*/
            				else error = true;
            			}else{/*not command after label*/
            				if((strcmp((words->word+words->size-1)->str,DATA_CMD)==0) || (strcmp((words->word+words->size-1)->str,STR_CMD)==0)){/*check if data command*/
            					if(addLb(label, DATA_LABEL))/*add data label*/
            						error = !handleIC(DATA,line,words);/*update data counter*/
            					else error = true;
            				}else if(strcmp(((words->word+words->size-1)->str),EXT_CMD) == 0){/*external line*/
          	  						if(getNextWordInLine(line,words)){/*get external label name*/
          	  							if(labelExist((words->word+words->size-1)->str)!=NULL){/*if label already exsist*/	
            									if(labelp->labelType!=EX_LABEL){/*if the lable type is not external*/
            										error = true;
            										fprintf(stdout,"error diffrent decleration of label : %s \n",(words->word+words->size-1)->str);	
            								}
            							}else {
            								error = !addLb((words->word+words->size-1)->str, EX_LABEL);/*add external label*/
            								}
            						}
            					}
            			}
            				}else {	error = true;
            						fprintf(stdout,"error in syntax\n");
            						}
			}else {
					if(isCmd((words->word+words->size-1)->str)!=-1){/*if its a command*/
            					error = !handleIC(INST,line,words);
            	}else{ 	
            			if((strcmp((words->word+words->size-1)->str,DATA_CMD)==0) || (strcmp((words->word+words->size-1)->str,STR_CMD) == 0)){/*not command after label*/
           						error = !handleIC(DATA,line,words);
            			}else{ 	
            					if(strcmp(((words->word+words->size-1)->str),EXT_CMD) == 0){/*external line*/
          	  						if(getNextWordInLine(line,words)){/*check the lable of the external line*/
          	  							labelp = labelExist((words->word+words->size-1)->str);
          	  							if(labelp!=NULL){/*if label already exsist*/	
            									if(labelp->labelType!= EX_LABEL){/*if the lable type is not external and exist in lable table*/
            										error = true;
            										fprintf(stdout,"error diffrent decleration of label : %s \n",(words->word+words->size-1)->str);
            								}	
            							}else {
            								error = !addLb((words->word+words->size-1)->str, EX_LABEL);
            								}
            						}
            					}else{ if(strcmp((words->word+words->size-1)->str,ENT_CMD) != 0){/*undeifined commend should not happen*/
            							 	fprintf(stdout,"unknown word: %s \n",(words->word+words->size-1)->str); 
            							 error = true; 
            							}
            						}
            			}
				}         
          	}
	}
	freeWords(words);/*free words allocation*/
	if(error)
		fprintf(stdout,"Row %d: The error is indicated in the row above. \n",findex);
            	
	return !error;	
}



/*input:	lnType lineType	: type
		char *line	: raw line from file
		lineWords *words: lineWords contain the words in the line in words->word array
handle insruction counter by the type of label
(INST = 'add','move...,DATA =.string "asf",.data 120,2, EXTERN = .extern)*/	
boolean handleIC(lnType lineType,char *line,lineWords *words){
	boolean error = false;		/*error boolean*/
	if(lineType == INST){
		IC++;/*instruction word*/
		if(getNextWordInLine(line,words)){
			IC++;	/*extra information for first operand*/
			switch (*((words->word+words->size-1)->str))
				{
					case '#':/*if first operand is direct number*/
					if(getNextWordInLine(line,words))
						IC++;
					break;
					
					case 'r':/*if the first operand starts with r can be register or label*/
					if((isReg((words->word+words->size-1)->str)!=-1)){
						if(getNextWordInLine(line,words)){
							if(!((isReg((words->word+words->size-1)->str)!=-1)||(((*((words->word+words->size-1)->str))=='*') && isReg((words->word+words->size-1)->str+1)))){/*check if the seconde word its not a register or a reference to reg (*r3 for exemple)*/
								IC++;
							}
						}
					}else	if(getNextWordInLine(line,words))/*if it starts with r and not a register its a label*/
								IC++;
						
					break;
					
					case '*':/*should be *register name*/
					if(getNextWordInLine(line,words))
						if((*((words->word+words->size-1)->str) != '*') && (isReg((words->word+words->size-1)->str) ==-1))/*check if second operand is not register*/
							IC++;
					break;
					
					default:/*first operand is lable*/
					if(getNextWordInLine(line,words))/*if second operand exist*/
						IC++;
				}
			}
	}else if(lineType == DATA){/*.data of ,string in the line*/
		if(strcmp((words->word+words->size-1)->str,DATA_CMD)==0){
            while(getNextWordInLine(line,words)){/*while there are stil variables*/
            		DC++;/*update data counter*/
            }
      	}else if(strcmp((words->word+words->size-1)->str,STR_CMD)==0){ 
            	if(getNextWordInLine(line,words)){
		      		if(isStr((words->word+words->size-1)->str))/*check if valid string*/
		      			DC+=strlen((words->word+words->size-1)->str) - 2 + 1;/*ignor " in the start and end and add \0*/
		        }else{	error = true;
		        		 fprintf(stdout,"error: no data was enterd\n");
		        	}
            }
	}
	return !error;

}




   
