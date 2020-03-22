#include "syntax.h"


int main(int argc, const char * argv[])
{
	int i=0;
	if(argc==1)
		printf("no file recived\n");
	else{
		for(i=1;i<argc;i++)
		{
			prossesAsm(argv[i]);
		}
		
	}
	return 1;
}

Boolean prossesAsm(const char* fileName)
{
	FILE *fp=NULL;
	fp = fopen(fileName, "r");
    	if (fp == NULL){
        	printf("No file found.\n");
        	return false;}
        else
        {
        	printf("\n %s \n",syntax_chack(fp)?"true":"false");
				
		fclose(fp);
		return true;
	}

	printf("finish");
}

/*Function*/
/*Input: Receives a reading line
Output: If correct returns 1 if incorrect returns 0*/
Boolean syntax_chack (FILE *fp)
{
	char buff[MAX_LINE];
	char *first_char=NULL, *last_char=NULL;
	char command [MAX_COMMAND] = {0}, zero_command[MAX_COMMAND] = {0}; 
	Opcode comm_checker;
	int row_number=0, comma_count=0, colon_count=0;
	Boolean flag = true, labled = false;

	while (fgets(buff,MAX_LINE, fp) != NULL)  /*A loop that goes through a file to the end.*/
	{
		row_number++;
		printf("%d.%s", row_number, buff);
		comma_count=0, colon_count=0, labled = false;
		count =0;
		strncpy(command,zero_command,MAX_COMMAND);
		
		/*4 rows to scans the first word*/
		first_char=buff;
		Next_First_Char(&first_char,&comma_count);
		
		if (*first_char=='\n'|| *first_char=='\0') /*Case of blank line.*/
			continue;

	    	if (def_lable (&first_char,&last_char,command, &comma_count, &colon_count,row_number) ==true)
	    	{
	    		labled = true;
	    		/*find the next word.*/
	    		first_char = last_char;
	    		Next_First_Char(&first_char,&comma_count); 	
	    		last_char=first_char;
	    		Next_last_Char(&last_char);	
	    	}
	    	
	    	
	    	else 
		{ 	
	    		comm_checker=Command_check(command); /*check if the first word is legal*/
	    		
	    		/*Check if the method has a source operand.*/
	    		switch (comm_checker)
	    		{
	    			case faile:
	    			{
	    				if (*first_char == ';') /*Case of comment statement, Nothing to do with the line*/
	    					break;
	    					
	    				if (*first_char == '.') /*A case of Instruction statement*/
	    				{
	    					if (strcmp(command, ".data")==0) /*Case of Instruction statement for data.*/
	    					{
	    						
   							if ((flag == true) && (next_num (&first_char, &last_char,&comma_count, row_number)== false))
	    							flag = false;
	    						
	    						if ((flag == true) && (to_many_arg(&last_char, row_number)== false))
	    							flag = false;
	    						
	    						break;
	    					}
	    					
	    					else if (strcmp(command, ".string")==0) /*Case of Instruction statement for string.*/
	    					{
	    						first_char = last_char;

	    						if((isString(&first_char , &last_char, &comma_count ,row_number )== false) && (flag == true))
	    							flag = false;
	    							
	    						if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    							flag = false;
	    					
	    									break;
	    					}
	    						
	    					
	    					else if ((strcmp(command, ".entry")==0) || (strcmp(command, ".extern")==0)) /*Case of Instruction statement for entry or extern.*/
	    					{
	    						if (labled == true)
	    						{
	    							if (strcmp(command, ".entry")==0)
	    								printf("Row %d: WORNING! '.entry' cannot be labled.\n", row_number);
	    								
	    							else
	    								printf("Row %d: WORNING! '.entry' cannot be labled.\n", row_number);
	    						}
	    						
	    						first_char = last_char;
	    						Next_First_Char(&first_char,&comma_count);	
	    						
	    						if(comma_count>0) /*if there was a comma in the first word or prior prints error*/
	    						{
	    							printf("Row %d: Illegal comma.\n", row_number);
	    							flag = false;
	    						}
	    						
	    						if((islable (&first_char, &last_char, row_number)==false) && (flag == true))
	    							flag = false;
	    							
	    						if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    							flag = false;
	    						
	    						break;	
	    							
	       					}
	    					
	    					else /*Case of Illegal Instruction statement.*/
	    					{
	    						printf("Row %d: Illegal guideline.\n", row_number);
	    						flag = false;
	    						
	    						break;	
	    					}
	    					
	    				}
	    				
	    			}
	    			
				/*4 source operand, 3 Target operand.*/
	    			case mov:
	    			case add: 
	    			case sub:
	    			{

	     				if ((addres_chack(&first_char, &last_char, 4, FIRST, &comma_count, row_number)== false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    					flag = false;
					
	    				if ((addres_chack(&first_char, &last_char, 3, SECOND, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    					flag = false;

	    				if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    					flag = false;
	    				
	    				break;
	    			} 
	    			
	    			/*4 source operand, 4 Target operand.*/
	    			case cmp:
	    			{
	    			
	    				if ((addres_chack(&first_char, &last_char, 4, FIRST, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    					flag = false;    	
	    						
	    				if ((addres_chack(&first_char, &last_char, 4, SECOND, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    					flag = false;
	    				
	    				if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    					flag = false;
	    					
	    					
	    				break;
	    			}
	    			
	    			/*1 source operand, 3 Target operand.*/
	    			case lea:
	    			{
	    				if ((addres_chack(&first_char, &last_char, 1, FIRST, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    					flag = false;
	    				
	    				if ((addres_chack(&first_char, &last_char, 3, SECOND, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    					flag = false;
	    				
	    				if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    					flag = false;
	    					
	    				break;
	    			}
	    			
	    			/*No source operand, 2 Target operand.*/
	    			case jmp:
	    			case bne:
	    			{
	    				/*Check if the flag needs to be updated.*/
					if ((addres_chack(&first_char, &last_char, 2, ONLY,&comma_count, row_number) == false) && (flag == true)) 
	    					flag = false;
	    				
	    				if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    					flag = false;
	    					
	    				break;
	    			}
	    			
	    			/*No source operand, 3 Target operand.*/
	    			case clr:
	    			case not:
	    			case inc:
	    			case dec:
	    			case red:
	    			case jsr:
	    			
	    			{
	    				/*Check if the flag needs to be updated.*/
					if ((addres_chack(&first_char, &last_char, 3, ONLY, &comma_count, row_number) == false) && (flag == true)) 
	    					flag = false;
	    				
	    				if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    					flag = false;
	    				break;
	    			}
	    			
	    			
	    			
	    			/*No source operand, 4 Target operand.*/
	    			case prn:
	    			{
	    				/*Check if the flag needs to be updated.*/
					if ((addres_chack(&first_char, &last_char, 4, ONLY, &comma_count, row_number) == false) && (flag == true)) 
	    					flag = false;
	    					
	    				if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    					flag = false;
	    				
	    				break;
	    			}
	    			
	    			/*No source operand, no target operand.*/
	    			case rts:
	    			case stop:
	    			{
	    				if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    					flag = false;
	    					
	    				break;
	    			}
	    			
			}
		}
			
	}
	return flag;
}	    		
	    		


/*Auxiliary functions*/

/*get to the next non \t or ' ' character*/
void Next_First_Char(char **first_char, int *comma_count)
{
	if(**first_char==',')/*counts if there is a comma*/
    			(*comma_count)++;
    			
	while(((**first_char=='\t') || (**first_char==' ')) && (**first_char!='\n') && ((**first_char!='\0')))
    	{
        	(*first_char)++;
        	if(**first_char==',')/*counts if there is a comma*/
    			(*comma_count)++;	
    	}
}

/*get to the next ',' \t or ' ' character*/
void Next_last_Char(char ** last_char)
{
	if (**last_char==',')
		(*last_char)++;
		
	while(**last_char!='\t' && **last_char!=' ' && **last_char!='\0' && **last_char!=',')
    		(*last_char)++;
    		
    	
    	if(**last_char=='\0') /* included to get the word that was inputed last properly*/
    	{
    		
    		(*last_char)--;
    	}
}


/*Input: The first and last character of the second word and number of checks.
Output: Verify if correct input, false if incorrect.*/	   		    	
Boolean addres_chack(char **first_char, char **last_char,int check_num ,int op_num, int *comma_count, int row_number)
{
	int len, comm_checker, op_position;
	char *operator = NULL;
	Boolean flag = false;
	
	/*4 lines to go to the first letter of the next word */
	*first_char=*last_char;		
    	Next_First_Char(first_char, comma_count);
    	*last_char=*first_char;
    	Next_last_Char(last_char);
	 
	
	
	 
	if ((op_num == ONLY) ||(op_num == FIRST))
		op_position=FIRST;
	 	  
	if (op_num == SECOND)
		op_position=SECOND;
	 	  
	if ((**first_char == '\n') || (**first_char == '\0'))
   	{
  		printf("Row %d: Missing argument.\n", row_number);
    		return false;
    	}
	  				
    	if(*comma_count<op_position) /*check if there is comma and stops if there is*/ 
		{
   			printf("Row %d: Missing comma.\n", row_number);
    			return false;  		
		}
	
	else if(*comma_count>op_position) /*check if there is comma and stops if there is*/ 
	{
		if (op_num==FIRST)
			printf("Row %d: Missing argument.\n", row_number);
		else
  			printf("Row %d: Illegal comma.\n", row_number);
    		return false;  		
	}
		
	
	else
	{			
		if (op_position == SECOND)
		{
			
			if (**first_char==',')
				(*first_char)++;
			
						
    			Next_First_Char(first_char, comma_count);
    			*last_char=*first_char;	
    			Next_last_Char(last_char);
    			 
	 
    			if ((**first_char == '\n') || (**first_char == '\0'))
   			{
  				printf("Row %d; Argument %d: Missing argument.\n", row_number, op_position+1);
    				return false;
    			}
    			
    		}
    		
    		
    		
    						    	
	    	/*0*/ if ((**first_char=='#') && (check_num == 4)) /*Check if this is an immediate address.*/ 
	    	{
			(*first_char)++; 
			*last_char=*first_char;	
					
					
			if (**first_char == '-')/*Check if the number is negative*/			(*last_char)++;		
			
			while ((**last_char>='0')&&(**last_char <= '9')) /*Passing over the number to the end*/
 			{
    				(*last_char)++;
       				flag = true;
    			}	
    			
    			while(**last_char!='\t' && **last_char!=' ' && **last_char!='\0' && **last_char!=',')
    			{
    				if (((**last_char<'0')||(**last_char > '9')) && (**last_char != '\n')&& (**last_char != '\0'))
    					flag = false;
    				
    				(*last_char)++;
    			}
    			
    			
    			if (flag == false) /* chack if the Immediate address is properly defined*/
    			{
    				printf("Row %d; Argument %d: Immediate address is not properly defined.\n", row_number, op_position+1);
    				Next_last_Char(last_char);
    				
    				return false;
    			}
    	
    			else
				return true;
		}
			    	
			
		/*2*/ else if ((**first_char=='*')&&((check_num=2)||(check_num=3)||(check_num=4))) /*Check if this is an indirect register addressing.*/
		{
	    		(*first_char)++; 
	    		len=*last_char-*first_char;
	    		operator = (char *)malloc((sizeof(char))*len);
	    		if (operator == NULL)
	    		{
	    			printf("Allocation error.\n");
	    			exit(1);
	    		}
			strncpy(operator, *first_char, len);
			    		
			if (isResinger(operator) == false) 
			{
			    	printf("Row %d; Argument %d: Indirect register addressing is not properly defined.\n", row_number, op_position+1);
			    	free(operator);
	    			return false;  
			}
			    	
			    	free(operator);	
			   	return true;
		}
			    	
 	
		len=*last_char-*first_char;
		operator = (char *)malloc((sizeof(char))*len);
	    	if (operator == NULL)
		{
			printf("Allocation error.\n");
    			exit(1);
	    	}
		
		strncpy(operator, *first_char, len);
		
		
		
		/*3*/ if ((isResinger(operator) == true)&&((check_num=3)||(check_num=4)))/*Check if this is a direct register addressing.*/
			{
				free(operator);	
				return true;
			}
		comm_checker=Command_check(operator);	
		if (comm_checker != faile) /*Check if the second word is a reserved word*/
		{
			printf("Row %d; Argument %d: The operator is a reserved word.\n", row_number, op_position+1);
			free(operator);
			return false;
		}
			
		/*1*/ if (islable (first_char, last_char, row_number)== true)/* Check if this is a direct addressing*/
			{
				
				free(operator);
				return true;
			}
			
		/*A case where none of the options are available*/
		printf("Row %d; Argument %d: The operand is not properly configured.\n", row_number, op_position+1);
			{
				free(operator);
				return false;
			}    			
	
	}
}    	



Boolean to_many_arg(char **last_char, int row_number)
{
	char **temp;
	temp = last_char;
	
 	while((**temp)=='\t' || (**temp)==' ')					
   		(*temp)++;
   			
	if((**temp)=='\n' || (**temp)=='\0')
      		return true;
      	
      	else if ((**temp)==',')
 		printf("Row %d: Illegal comma.\n", row_number);
		
      	else	
      		printf ("Row: %d: To many arguments.\n", row_number);
	
	return false;
		
}
	
Boolean def_lable (char ** first_char, char ** last_char, char* command, int * comma_count, int* colon_count,  int row_number)
{
	int len=0;
	Boolean correct_lable = false;
	
	correct_lable = islable (first_char, last_char, row_number);

	while(**last_char!='\t' && **last_char!=' ' && **last_char!='\0' && **last_char!='\n' && **last_char!='\0')
    		{
    			if(**last_char==':') /*counts if there is a comma*/
    				(*colon_count)++;
    			
    			(*last_char)++;
    		}
    		
    	if(**last_char==':') /*counts if there is a comma*/
    	{
    		(*last_char)++;	
    		(*colon_count)++;
    	}
    	
    	len=*last_char-*first_char;
    	
    	strncpy(command, *first_char, len);
    	printf("command:%s\n", command);
    	
    	if ((Command_check(command)!=faile)||(!strcmp(command,".data")) ||(!strcmp(command,".string")) ||(!strcmp(command,".extern")) ||(!strcmp(command,".entry")))
    		return false;
    	
    	
    	if (isReservWord(command, row_number)==true)
    		return false;
    	
	if(correct_lable==true)/*counts if there is a comma*/
    	{	
    		if ((**last_char ==':') && (*colon_count==1))
    			{	printf("correct_lable: %d\n", correct_lable);
    			return true;
    		}
    		
    		else
    		{
    			printf("Row %d: Laibel is not properly define.\n", row_number);
    			return false;
		}
    	}
    	
    	else
    	{
    		printf("Row %d: Laibel is not properly define.\n", row_number);
    			return false;
    	}
}

Boolean islable (char **first_char,char **last_char, int row_number)
{
	*last_char = *first_char;
	
	while ((((**last_char)>='0' && (**last_char)<='9') || ((**last_char)>='A' && (**last_char)<='Z') || ((**last_char)>='a' && (**last_char)<='z')) && (**last_char!='\n') && (**last_char!='\0')) /*Enter the loop only if the word consists of letters or numbers*/
	{	
		(*last_char)++;
		
		if ((*last_char - *first_char) > MAX_LABLE)
		{
			printf ("Row: %d: Invalid label.\n", row_number);
			return false;
		}
	}
	
	return true;
}

Boolean isString (char **first_char, char **last_char, int *comma_count, int row_number)
{
	*first_char = *last_char;
	Next_First_Char(first_char,comma_count); /*Skip white characters*/
	*last_char = *first_char;
	
	if ((**first_char) == '"') 
	{	
		(*last_char)++;
		
		while ((**last_char) != '"' )
	 	{
			(*last_char)++;
	    		if (((**last_char) == '\n') || ((**last_char) == '\0')) /*Checking for invalid new line in string.*/
	    		{
	    			printf("Row %d: Illegal sting, missing quotation marks.\n", row_number);
	    			return false;	
	    		}	
	    	}
	    	
	    	return true;
	}
		

	printf("Row %d: Expected to string.\n", row_number);
 		return false;
}

Boolean next_num (char **first_char, char **last_char,int * comma_count, int row_number)
{
	Boolean flag = true, temp_flag;
	int count=0;

	
	
	while((count == *comma_count) && (**first_char != '\n') && (**first_char != '\0') && ( flag == true)) 
	{
		
		
		
		*first_char=*last_char;
		Next_First_Char(first_char,comma_count); /*Skip white characters*/
		*last_char=*first_char;
		
		
		temp_flag = isNum (last_char, row_number);
		
		
		if ( temp_flag== true)
			count++;
			
		else
		{
			if (flag == true) 
				flag = false;
		}
		
		*first_char=*last_char;
		Next_First_Char(first_char, comma_count); /*Skip white characters*/
		*last_char=*first_char;
		Next_last_Char (last_char);
		 
		
		
	}
	
	if (count == *comma_count + 1)
		return true;
		
	else if (count < *comma_count + 1)
	{
		printf("Row %d: Missing comma.\n", row_number);
    		return false;
	}
	
	else
	{
		printf("Row %d: Illegal comma.\n", row_number);
    		return false;
	}
		
}
	

Boolean isNum (char **last_char, int row_number)
{
	Boolean flag = false;
	
	if (**last_char == ',')
		(*last_char)++;	
		
	if ((**last_char == '+')||(**last_char== '-'))
		(*last_char)++;	
	
	
	while((**last_char>='0')&&(**last_char <= '9'))
	{
		(*last_char)++;
		flag = true;
	}
		
	
	if((flag == true) && ((**last_char == ',') || (**last_char == ' ') || (**last_char== '\t') || (**last_char == '\n') || (**last_char == '\0')))
		return true;
	
	else
	{
		printf("Row %d: Immediate address is not properly defined.\n", row_number);
		
		
		return false;
		
	}
	
}

/*check whitch set was inputed */
Boolean isResinger (char * resinger)
{
	if((strcmp(resinger,"r0")==0) || (strcmp(resinger,"r1")==0)||(strcmp(resinger,"r2")==0) || (strcmp(resinger,"r3")==0) || (strcmp(resinger,"r4")==0) || (strcmp(resinger,"r5")==0) || (strcmp(resinger,"r6")==0) || (strcmp(resinger,"r7")==0))
	{
		return true;
	}
	
	return false;
}

/*check whitch command was inputed */
Opcode Command_check(char * command)
{
	if(!strcmp(command,"mov")) 
		return mov;
		
	else if(!strcmp(command,"cmp")) 
		return cmp;
	
	else if(!strcmp(command,"add"))
		return add;
	
	else if(!strcmp(command,"sub"))
		return sub;
	
	else if(!strcmp(command,"lea"))
		return lea;
	
	else if(!strcmp(command,"clr"))
		return clr;
	
	else if(!strcmp(command,"not"))
		return not;
	
	else if(!strcmp(command,"inc"))
		return inc;
	
	else if(!strcmp(command,"dec"))
		return dec;
	
	else if(!strcmp(command,"jmp"))
		return jmp;
	
	else if(!strcmp(command,"bne"))
		return bne;
	
	else if(!strcmp(command,"red"))
		return red;
	
	else if(!strcmp(command,"prn"))
		return prn;
	
	else if(!strcmp(command,"jsr"))
		return jsr;
	
	else if(!strcmp(command,"rts"))
		return rts;
	
	else if(!strcmp(command,"stop"))
		return stop;
	else
		return faile;
}

Boolean isReservWord(char* word,int row_number)
{
	if ((!strcmp(word,"mov:")) || (!strcmp(word,"cmp:")) ||(!strcmp(word,"add:")) ||(!strcmp(word,"sub:")) ||(!strcmp(word,"lea:")) ||(!strcmp(word,"clr:")) ||(!strcmp(word,"not:")) ||(!strcmp(word,"inc:")) ||(!strcmp(word,"dec:")) ||(!strcmp(word,"jmp:")) ||(!strcmp(word,"bne:")) ||(!strcmp(word,"red:")) ||(!strcmp(word,"prn:")) ||(!strcmp(word,"jsr:")) ||(!strcmp(word,"rts:")) ||(!strcmp(word,"stop:")) || (!strcmp(word,".data:")) ||(!strcmp(word,".string:")) ||(!strcmp(word,".extern:")) ||(!strcmp(word,".entry:"))||(!strcmp(word,"r0:"))||(!strcmp(word,"r1:"))||(!strcmp(word,"r2:"))||(!strcmp(word,"r3:"))||(!strcmp(word,"r4:"))||(!strcmp(word,"r5:"))||(!strcmp(word,"r6:"))||(!strcmp(word,"r7:")))
	{	
		printf("Row %d: Illegal laibel, reserved word.\n", row_number);
		return true;
	}
	
	else
		return false;
}
