#include "SecondPassUtils.h"

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
            						printf("error: unknown word: %s \n",(words->word+words->size-1)->str);
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
            			printf("unknown word: %s",(words->word+words->size-1)->str); 
            			error = true;
            		}
           }
        freeWords(words);
        if(error)
		printf("Row %d: error in row.\n",findex);
	return !error;/*retun no error has occurd in line*/	
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

/*input:char *line 	: 	raw line from file
	lineWords *words: 	an array of words fill with line words until INSTRUCTION command
output: boolean value	: 	succesfully update RAM and external table
the function update tha RAM and external table accourding INST line type*/
boolean InstRAMWords(char *line,lineWords *words){
		
	unsigned int RAMWord[MAX_WORD_INST]={0};/*an array that contain the words to add to the RAM*/
	int i=0,numWords=1;/*i: index, numWords: the number of words to add to the RAM*/
	int reg=-1;/*register variable,setes to -1,-no found*/
	labelAd *label=NULL;/*label pointer*/
	RAMWord[0]|=(isCmd((words->word+words->size-1)->str)<<OCAM_SBIT);/*add opcode */
	RAMWord[0]|=A_THREE;/*turn on the A bit from ARE*/ 
       	if(getNextWordInLine(line,words)){/*if there is a next word in line*/
      		switch (*((words->word+words->size-1)->str))/*check the firs cher of operand*/
			{
				case '#':
				RAMWord[1]=getDirectWord((words->word+words->size-1)->str);/*get the informatio word for # operand*/ 
				numWords++;/*add information word*/
				if(getNextWordInLine(line,words)){
					RAMWord[0]|=(A_ONE<<FOAM_SBIT);/*adress methode of the first operand*/
					numWords++;/*add second operand information word*/
					label=labelExist((words->word+words->size-1)->str);/*check if second operand is label*/
					if(label!=NULL){/*if the second operand is a label*/
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
							}else if (*((words->word+words->size-1)->str) == '#' ){
								RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode of the first operand*/
								RAMWord[2]=getDirectWord((words->word+words->size-1)->str);/*get the informatio word for # operand*/ 
								}else{	printf("error: %s unknown operator\n",(words->word+words->size-1)->str);
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
												}else if (*((words->word+words->size-1)->str) == '#' ){
																RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode of the first operand*/
																RAMWord[2]=getDirectWord((words->word+words->size-1)->str);/*get the informatio word for # operand*/ 
												}else {printf("error: unknown operand\n");
																return false;
															}
											}
										}
									}
								}else{	if(reg != -1){
										RAMWord[0]|=(A_FOUR<<SOAM_SBIT);
										RAMWord[1] = getRegWord(reg,SOR_SBIT);
									}else if(label!=NULL)
										RAMWord[0]|=(A_TWO<<SOAM_SBIT);
									} 
						
				break;
			
				case '*':/*type of operanf *register*/
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
									}else if (*((words->word+words->size-1)->str) == '#' ){/*check direct numnber*/
														RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode of the first operand*/
														RAMWord[2]=getDirectWord((words->word+words->size-1)->str);/*get the informatio word for # operand*/ 
								}else{ 	printf("error: unknown operatopr\n");
											return false;
											}
								}
							}
						}else {	RAMWord[0]|=(A_THREE<<SOAM_SBIT);
							RAMWord[1] = getRegWord(reg,SOR_SBIT);
							}
					}else{ 	printf("error: wrong in put * before reg\n");
							return false;
							}
				break;
				
				default:/*first operand is a label*/
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
									}else if (*((words->word+words->size-1)->str) == '#' ){
														RAMWord[0]|=(A_ONE<<SOAM_SBIT);/*adress methode of the first operand*/
														RAMWord[2]=getDirectWord((words->word+words->size-1)->str);/*get the informatio word for # operand*/ 
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




/*input : int reg : the number of the register
	  int offsett : offset of the reg value in the new information word
output	: int value : value of a information word of register  word
the function get label and returns the RAM information word */
int getRegWord(int reg,int offset){
	
	unsigned int word=0;
	word|= reg<<offset;/*allocate the register in the offset bit place*/
	word|=A_THREE;/*turn on the A bit from ARE*/
	return word;
}

/*input : labelAd* label : label to get information word from
output	: int value : value of a information word of label value word
the function get label and returns the RAM information word */
int getLabelWord(labelAd* label){
	
	unsigned int word=0;
	word|=((label->address)<<EO_ARE);/*label address in left 12 bits*/
	if(label->labelType==EX_LABEL)
		word|=A_ONE;/*turn on the R bit from ARE*/
	else	word|=A_TWO;/*turn on the E bit from ARE*/
	return word;
}

/*input : char* num : a string that contain number
output	: int value : value of a information word of direct value word
the function get string num and returns the RAM information word */
int getDirectWord(char *num){
	
	unsigned int word=0;
	word|=(atoi(num+1)<<EO_ARE);/*move the value to the last 12 bits*/
	word|=A_THREE;/*turn on the A bit from ARE*/
	return word; 
}

