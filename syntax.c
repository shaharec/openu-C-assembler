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
Output: Return true if the file is correct and return false and print the error essence if the input is incorrect.*/
Boolean syntax_chack (FILE *fp)
{	
	char buff[MAX_LINE];
	char *first_char=NULL, *last_char=NULL;
	char command [MAX_COMMAND] = {0}, zero_command[MAX_COMMAND] = {0}; 
	Opcode comm_checker;
	int row_number=0, comma_count=0, colon_count=0, len=0;
	Boolean flag = true, labled = false;
	
	/* A loop that receives from a text file and runs to the end of the file. */ 
	while (fgets(buff,MAX_LINE, fp) != NULL)
	{
		/*Reset variables.*/
		row_number++;
		printf("%d.%s", row_number, buff);
		comma_count=0, colon_count=0, labled = false;
		count =0;
		strncpy(command,zero_command,MAX_COMMAND);
		
		/*Find the first character in the next word.*/
		first_char=buff;
		Next_First_Char(&first_char,&comma_count);
		
		/*A case of a line of white characters or a line that begins with a semicolon.*/
		if (*first_char=='\n'|| *first_char=='\0' || *first_char==';') 
			continue;

		/*Check if a label is set, if so the instruction word is recorded to the "Command" argument . If you do not receive the instruction, the argument is "Command"*/
	    	if (def_lable (&first_char,&last_char,command, &comma_count, &colon_count,row_number) ==true)
	    	{
	    		labled = true; /*There is a label*/
	    		
	    		/*Find the next word.*/
	    		first_char = last_char;
	    		Next_First_Char(&first_char,&comma_count); 	
	    		last_char=first_char;
	    		Next_last_Char(&last_char);
	    		
	    		strncpy(command,zero_command,MAX_COMMAND); /*Resetting the "command" argument array*/
	    		
	    		/*Placing a string in the "command" array.*/
	    		len=last_char-first_char;
    			strncpy(command, first_char, len);
	    			
	    	}
	    	
	    	/*Saving a number representing a command. Performed using the function "Command_check"*/	
	    	comm_checker=Command_check(command);
	    	
	    	/*"Switch" command to determine how to handle the command.*/
	    	switch (comm_checker)
	    	{
	    		case guid_data: /*.data command*/
	    		{
	    			/*Check if the parameters are set correctly for the directive sentence.*/			
   				if ((next_num (&first_char, &last_char,&comma_count, row_number)== false) && (flag == true))
	    				flag = false;
	    			
	    			/*Check if there are additional arguments in the line and update the flag.*/
	    			/*if ((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    				flag = false;*/
	    						
	    			break;
	    		}
	    			
	    		case guid_string:/*.string command*/
	    		{
				/*Check if the string is set correctly for the directive sentence.*/
	    			if((isString(&first_char , &last_char, &comma_count ,row_number )== false) && (flag == true))
	    				flag = false;
	    					
	    			break;
	    		}
	    			
	    		case guid_entry: /*.entry command*/
	    		{	
	    			/*Check if a label is set for the directive sentence.*/
	    			if (labled == true)
	    				printf("Row %d: WORNING! '.entry' cannot be labled.\n", row_number);
	    			
	    			/*Find the first character in the next word.*/			
	    			first_char = last_char;
	    			Next_First_Char(&first_char,&comma_count);	
	    			
	    			/*If there was a comma in the first word and prints error if so and update the flag.*/		
	    			if(comma_count>0)
	    			{
	    				printf("Row %d: Illegal comma.\n", row_number);
	    				flag = false;
	    				
	    				/*Find the first character in the next word.*/
	    				first_char++;
	    				Next_First_Char(&first_char,&comma_count);
	    			}
	    			
	    			/*Check if a semicolon is set before checking the operators then an error message will print and move to the next iteration of the main loop.*/
	    			if (*first_char == ';')
	    			{
	    				printf("Row %d: Invalid command line.\n", row_number);
	    				flag = false;
	    						
	    				break;
	    			}
	    		
	    			/*Check if the argument is a valid label and update the flag.*/			
	    			if((islable (&first_char, &last_char, row_number)==false) && (flag == true))
	    				flag = false;
	    			
	    			/*Check if there are additional arguments in the line and update the flag.*/				
	    			if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    				flag = false;
	    						
	    			break;	
	    					
	       		}
	    			
	    		case guid_extern: /*.extern command*/
	    		{
	    			/*Check if a label is set for the directive sentence.*/
	    			if (labled == true)
	 				printf("Row %d: WORNING! '.extern' cannot be labled.\n", row_number);		
	    			
	    			/*Find the first character in the next word.*/
	    			first_char = last_char;
	    			Next_First_Char(&first_char,&comma_count);	
	    			
	    			/*If there was a comma in the first word and prints error if so and update the flag.*/			
	    			if(comma_count>0)
	    			{
	    				printf("Row %d: Illegal comma.\n", row_number);
	    				flag = false;
	    				
	    				first_char++;
	    				Next_First_Char(&first_char,&comma_count);
	    			}
	    			
	    			/*Check if a semicolon is set before checking the operators then an error message will print and move to the next iteration of the main loop.*/	
	    			if (*first_char == ';')
	    			{
	    				printf("Row %d: Invalid command line.\n", row_number);
	    				flag = false;
	    						
	    				break;
	    			}
	    			
	    			/*Check if the argument is a valid label and update the flag.*/		
	    			if((islable (&first_char, &last_char, row_number)==false) && (flag == true))
	    				flag = false;
	    			
	    			/*Check if there are additional arguments in the line and update the flag.*/				
	    			if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    				flag = false;
	    						
	    			break;	
	    					
	       		}
	    			
	    		/*4 source operand, 3 Target operand.*/
	    		case com_mov: /*mov command*/
	    		case com_add: /*add command*/
	    		case com_sub: /*sub command*/
	    		{
	    			/*Checking the first argument and updating the flag.*/
	     			if ((addres_chack(&first_char, &last_char, 4, FIRST, &comma_count, row_number)== false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    				flag = false;
					
				/*Checking the second argument and updating the flag.*/		
	    			if ((addres_chack(&first_char, &last_char, 3, SECOND, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    				flag = false;
				
				/*Check if there are additional arguments in the line and update the flag.*/
	    			if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    				flag = false;
	    				
	    			break;
	    		} 
	    			
	    		/*4 source operand, 4 Target operand.*/
	    		case com_cmp:/*cmp command*/
	    		{
	    			/*Checking the first argument and updating the flag.*/
	    			if ((addres_chack(&first_char, &last_char, 4, FIRST, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    				flag = false;    	
	    			
	    			/*Checking the second argument and updating the flag.*/				
	    			if ((addres_chack(&first_char, &last_char, 4, SECOND, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    				flag = false;
	
	    			break;
	    		}
	    			
	    		/*1 source operand, 3 Target operand.*/
	    		case com_lea:
	    		{
	    			/*Checking the first argument and updating the flag.*/
	    			if ((addres_chack(&first_char, &last_char, 1, FIRST, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    				flag = false;
	    				
	    			/*Checking the second argument and updating the flag.*/	
	    			if ((addres_chack(&first_char, &last_char, 3, SECOND, &comma_count, row_number) == false) && (flag == true)) /*Check if the flag needs to be updated.*/
	    				flag = false;
	    			
	    			/*Check if there are additional arguments in the line and update the flag.*/	
	    			if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    				flag = false;
	    					
	    			break;
	    		}
	    			
	    		/*No source operand, 2 Target operand.*/
	    		case com_jmp:/*jmp command*/
	    		case com_bne:/*ben command*/
	    		{
	    			/*Test the only argument and update the flag.*/
				if ((addres_chack(&first_char, &last_char, 2, ONLY,&comma_count, row_number) == false) && (flag == true)) 
	    				flag = false;
	    			
	    			/*Check if there are additional arguments in the line and update the flag.*/	
	    			if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    				flag = false;
	    					
	    			break;
	    		}
	    			
	    		/*No source operand, 3 Target operand.*/
	    		case com_clr:/*clr command*/
	    		case com_not:/*not command*/
	    		case com_inc:/*inc command*/
	    		case com_dec:/*dec command*/
	    		case com_red:/*red command*/
	    		case com_jsr:/*jsr command*/
	    		{
	    			/*Test the only argument and update the flag.*/
				if ((addres_chack(&first_char, &last_char, 3, ONLY, &comma_count, row_number) == false) && (flag == true)) 
	    				flag = false;
	    			
	    			/*Check if there are additional arguments in the line and update the flag.*/
	    			if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    				flag = false;
	    			break;
	    		}
	    			
	    		/*No source operand, 4 Target operand.*/
	    		case com_prn:/*prn command*/
	    		{
	    			/*Test the only argument and update the flag.*/
				if ((addres_chack(&first_char, &last_char, 4, ONLY, &comma_count, row_number) == false) && (flag == true)) 
	    				flag = false;
	    			
	    			/*Check if there are additional arguments in the line and update the flag.*/	
	    			if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    				flag = false;
	    				
	    			break;
	    		}
	    			
	    		/*No source operand, no target operand.*/
	    		case com_rts:/*rts command*/
	    		case com_stop:/*stop command*/
	    		{
	    			/*Check if there are additional arguments in the line and update the flag.*/
	    			if((to_many_arg(&last_char, row_number)== false) && (flag == true))
	    				flag = false;
	    					
	    			break;
	    		}
	    		
	    		/*If command is no specified*/
	    		default:
	    		{
	    			printf("Row %d: Invalid command line.\n", row_number);
	    					flag = false;
	    						
	    			break;	
	    		}
	    			
		}
		
			
	}
	return flag;
}	    		
	    		


/*Auxiliary functions*/

/*Input: Pointer for beginning word and comma count.
Output: will advance the pointer to the beginning of the word, return true if there is ';' In the section, will return false if not. If a comma encounter , it will update the comma counter*/
void Next_First_Char(char **first_char, int *comma_count)
{	
	/*Counts if there is a comma*/
	if(**first_char==',')
    		(*comma_count)++;
    	
    	/*Loop runs on white characters and stops if you meet non-white or end-of-line character.*/		
	while(((**first_char=='\t') || (**first_char==' ')) && (**first_char!=';') && (**first_char!='\n') && ((**first_char!='\0')))
    	{
        	(*first_char)++;
        	
        	if(**first_char==',')/*counts if there is a comma*/
    			(*comma_count)++;	
    	}
}

/*Input: Pointer to the end of a word.
Output: Will advance the pointer to the end of the word, stop if it has a white space, ';' Or ','*/
void Next_last_Char(char ** last_char)
{
	/*If the first character is a comma, the pointer will move forward.*/
	if (**last_char==',')
		(*last_char)++;
	
	/*Loop runs on white characters and stops if you meet non-white or end-of-line character.*/	
	while(**last_char!='\t' && **last_char!=' ' && **last_char!='\0' && **last_char!=',' && **last_char!=';')
    		(*last_char)++;
    		
    	/* included to get the word that was inputed last properly*/
    	if(**last_char=='\0') 
    		(*last_char)--;  
}


/*Input: The first and last character for the operator word,  and number of checks to be able to do, how many operators the command have, comma counter and the number of the row.
Output: Verify if correct input for the operator, false if incorrect. Promotes pointer values.*/		    	
Boolean addres_chack(char **first_char, char **last_char,int check_num ,int op_num, int *comma_count, int row_number)
{
	int len, comm_checker, op_position;
	char *operator = NULL;
	Boolean flag = false;
	
	/*Promotions indicate the beginning of the first character of the word and the last letter of the word. */
	*first_char=*last_char;		
    	Next_First_Char(first_char, comma_count);
    	*last_char=*first_char;
    	Next_last_Char(last_char);
	
	/*Check if the argument is the first or only operator.*/ 
	if ((op_num == ONLY) ||(op_num == FIRST))
		op_position=FIRST;
	/*Check if the argument is the second operator.*/	  
	if (op_num == SECOND)
		op_position=SECOND;
	 
	 /*Check if we have reached the end of the line or a semicolon.*/	  
	if ((**first_char == ';') ||(**first_char == '\n') || (**first_char == '\0'))
   	{
  		printf("Row %d: Missing argument.\n", row_number);
    		return false;
    	}
	  
	/*If a comma is missing, an error message is printed and a false is returned.*/				
    	if(*comma_count<op_position) /*check if there is comma and stops if there is*/ 
		{
   			printf("Row %d: Missing comma.\n", row_number);
    			return false;  		
		}
	
	/*Check for excess comma error message and return false.*/
	else if(*comma_count>op_position) 
	{
		if (op_num==FIRST)/*Check if the argument is the first or only operator.*/
			printf("Row %d: Missing argument.\n", row_number);
		
		else /*Check if the argument is the second operator and return false.*/
  			printf("Row %d: Illegal comma.\n", row_number);
    		return false;  		
	}
		
	
	else /*If there is a comma exactly as defined, we will check the operators.*/
	{	
		/*If this is the second argument.*/		
		if (op_position == SECOND)
		{
			/*If the first character is a comma, the pointer will move forward.*/
			if (**first_char==',')
				(*first_char)++;
			
			/*Promotions indicate the beginning of the first character of the word and the last letter of the word. */			
    			Next_First_Char(first_char, comma_count);
    			*last_char=*first_char;	
    			Next_last_Char(last_char);
    			 
	 		/*Checking whether we have reached the end of the line or a semicolon and an error message is returned.*/
    			if ((**first_char == ';') ||(**first_char == '\n') || (**first_char == '\0'))
   			{
  				printf("Row %d; Argument %d: Missing argument.\n", row_number, op_position+1);
    				return false;
    			}
    			
    		}
    		
    		/*Check if this is an immediate address.*/				    	
	    	if ((**first_char=='#') && (check_num == 4))  
	    	{
			(*first_char)++; 
			*last_char=*first_char;			
					
			if (**last_char == '-')/*Check if the number is negative*/		
				(*last_char)++;		
			
			/*Passing the pointer over the number to the end*/
			while ((**last_char>='0')&&(**last_char <= '9')) 
 			{
    				(*last_char)++;
       				flag = true;
    			}	
    			
    			/*Checking whether we have reached the end of the line or a semicolon and an error message is returned.*/
    			while(**last_char!=';' && **last_char!='\n' && **last_char!='\0' && **last_char!=',')
    			{
    				/*If it is not a white character or number we will update the flag.*/
    				if (((**last_char<'0')||(**last_char > '9')) && **last_char!='\t' && **last_char!=' ')
    					flag = false;
    				
    				(*last_char)++;
    			}
    			
    			/*Chack if the Immediate address is properly defined*/
    			if (flag == false) 
    			{
    				printf("Row %d; Argument %d: Immediate address is not properly defined.\n", row_number, op_position+1);
    				Next_last_Char(last_char);/*Promote the last character to the next white character or comma or semicolon or sentence end.*/
    				
    				return false;
    			}
    	
    			/*If the operator is set correctly, the truth will be returned.*/
    			else 
				return true;
		}
			    	
		/*Check if this is an indirect register addressing.*/	
		else if ((**first_char=='*')&&((check_num=2)||(check_num=3)||(check_num=4))) 
		{	
	    		(*first_char)++; 
	    		
	    		/*Finding the word length and allocate a dynamic memory for a variable.*/
	    		len=*last_char-*first_char;
	    		operator = (char *)malloc((sizeof(char))*len);
	    		
	    		/*Check if the allocation was done correctly.*/
	    		if (operator == NULL)
	    		{
	    			printf("Allocation error.\n");
	    			exit(1);
	    		}
	    		
	    		/*Saving a value in a dynamic memory variable.*/
			strncpy(operator, *first_char, len);
			  
			/*Check if the value is Register if no appropriate error message is printed and false returned.*/  		
			if (isResinger(operator) == false) 
			{
			    	printf("Row %d; Argument %d: Indirect register addressing is not properly defined.\n", row_number, op_position+1);
			    	free(operator);
	    			return false;  
			}
			    	
			    	free(operator);	
			   	return true;
		}
			    	
 		/*Finding the word length and allocate a dynamic memory for a variable.*/
		len=*last_char-*first_char;
		operator = (char *)malloc((sizeof(char))*len);
	    	
	    	/*Check if the allocation was done correctly.*/
	    	if (operator == NULL)
		{
			printf("Allocation error.\n");
    			exit(1);
	    	}
		
		/*Saving a value in a dynamic memory variable.*/
		strncpy(operator, *first_char, len);
		
		
		/*Check if this is a direct register addressing.*/
		if ((isResinger(operator) == true)&&((check_num=3)||(check_num=4)))
			{
				free(operator);	
				return true;
			}
		
		/*Saving a number representing a operator. Performed using the function "Command_check"*/
		comm_checker=Command_check(operator);	
		
		/*Check if the second word is a reserved word*/
		if ((comm_checker>= guid_data ) && (comm_checker <= guid_extern)) 
		{
			printf("Row %d; Argument %d: The operator is a reserved word.\n", row_number, op_position+1);
			free(operator);
			return false;
		}
		
		/*(Direct addressing method) Checking whether the argument is a valid label and updating the flag.*/
		if (islable (first_char, last_char, row_number)== true)
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


/*Input: Pointer to end of word and row number.
Output: Moving on the characters to the end of the line and  return true if they are white spaces or a note. else false.*/
Boolean to_many_arg(char **last_char, int row_number)
{
	char **temp;
	temp = last_char;
	
 	while((**temp)=='\t' || (**temp)==' ')					
   		(*temp)++;
   			
	if((**temp)==';' ||(**temp)=='\n' || (**temp)=='\0')
      		return true;
      	
      	else if ((**temp)==',')
 		printf("Row %d: Illegal comma.\n", row_number);
		
      	else	
      		printf ("Row: %d: To many arguments.\n", row_number);
	
	return false;
		
}
	
/*Input: Gets Pointer to the beginning and end of a word, comma counter, colon counter and row number.
Output: Returns true if the first word in the line is a label, false if otherwise. Will update the counters if necessary and save the first word value in a row in the "command" variable. Promotes pointer values.*/
Boolean def_lable (char ** first_char, char ** last_char, char* command, int * comma_count, int* colon_count,  int row_number)
{
	int len=0, comm_checker;
	Boolean correct_lable = false;
	char *temp=NULL;
	
	/*Check if the argument is a valid label and update the flag.*/
	correct_lable = islable (first_char, last_char, row_number);
	
	if (correct_lable == false)
		Next_last_Char(last_char);
		
	temp = *last_char;
	
	while((*temp=='\t' || *temp==' ') && *temp!=':' && *temp!=';' && *temp!='\n' && *temp!='\0')
    		temp++;
    	
    		
    	if(*temp==':') /*counts if there is a comma*/
    	{
    		temp++;	
    		(*colon_count)++;
    	}
    	
    	len=*last_char-*first_char;
    	
    	strncpy(command, *first_char, len);
    	
    	comm_checker=Command_check(command);
    	*last_char = temp;
    	
    	if ((comm_checker>= com_mov) && (comm_checker <= guid_extern))
    		return false;
    	
    	
    	if (isReservWord(command, row_number)==true)
    		return false;
    	
	if(correct_lable==true)/*counts if there is a comma*/
    	{	
    		if ((*((*last_char)-1) ==':') && (*colon_count==1))
    			return true;
        		
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

/*Input: Gets Pointer to the beginning and end of a word and row number.
Output: Returns true if the received word is correct in terms of the lable syntax. False if not*/
Boolean islable (char **first_char,char **last_char, int row_number)
{
	*last_char = *first_char;
	if (**last_char == '.')
		return false;
		
	while ((((**last_char)>='0' && (**last_char)<='9') || ((**last_char)>='A' && (**last_char)<='Z') || ((**last_char)>='a' && (**last_char)<='z')) && (**last_char!=';') && (**last_char!='\n') && (**last_char!='\0')) /*Enter the loop only if the word consists of letters or numbers*/
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

/*Input: Gets Pointer to start and end of argument, comma counter and row number.
Output: Returns true if the received row sets a valid string. False if not. Promotes pointer values*/
Boolean isString (char **first_char, char **last_char, int *comma_count, int row_number)
{
	int quote_count=0;
	Boolean flag = true;
	
	*first_char = *last_char;
	Next_First_Char(first_char, comma_count);
	*last_char=*first_char;
	
	if ((**first_char) == '\"') 
	{	
		quote_count++;
		(*last_char)++;
		
		while((**last_char != '\n') && (**last_char != '\0'))
		{	
			if ((**last_char) == '\"')
				quote_count++;
		
			(*last_char)++;
		}
		
		
		
		while(**last_char != '\"')
		{
			(*last_char)--;
			
			if ((**last_char != '\"') && (**last_char != '\t') && (**last_char != ' ') && (**last_char != '\n') && (**last_char != '\0'))
				flag = false;
				
			if(**last_char == ';')
				flag = true;	
		}
		
		if ((quote_count>=2) && (flag == true))
			return true;
			
		else if (quote_count<2)
			printf("Row %d: Illegal sting, missing quotation marks.\n", row_number);
	    		
		else if ((quote_count>=2) && (flag == false))
			printf("Row %d: Too many arguments.\n", row_number);
		
		return false;
	}
		
	else	
	{
		printf("Row %d: Expected to string.\n", row_number);
 		return false;
 	}
}

/*Input: Gets Pointer to the beginning and end of an argument, comma counter and row number.
Output: Returns true if the received row is set correctly as a row of numbers separated by commas. Otherwise return false. Promotes pointer values*/
Boolean next_num (char **first_char, char **last_char,int * comma_count, int row_number)
{
	Boolean flag = true, num_flag = false;
	int count=0;

	
	*first_char=*last_char;
	Next_First_Char(first_char,comma_count); /*Skip white characters*/
	*last_char=*first_char;
	
	if (**last_char == ';')
	{
		printf("Row %d: Number missing.\n", row_number);
    		return false;
	}
	
	while((**last_char != ';') && (**last_char != '\n') && (**last_char != '\0')&& (**last_char != ';')) 
	{	
		
		num_flag = isNum (last_char, row_number);
		
		if (num_flag == true)
			count++;
		
		else
		{
			if (flag == true) 
				flag = false;
		}
		
		if (**last_char == ',')
		{
			(*comma_count)++;
			(*last_char)++;	
		}
		
		*first_char=*last_char;
	}
	
	
	if (flag == true)
	{
		if ((count == *comma_count + 1))
			return true;
		
		else if (count < *comma_count + 1)
		{
			printf("Row %d: Illegal comma.\n", row_number);
    			return false;
		}
	
		else
		{
			printf("Row %d: Missing comma.\n", row_number);
    			return false;
		}
	}
	
	else
	{
		printf("Row %d: Illegal symbol.\n", row_number);
    		return false;
	}		
}

	
/*Input: Gets Pointer to the beginning and end of an argument, comma counter and line number.
Output: Returns true if the argument sent is true number, otherwise false.
Promotes pointer values untill the next comma.*/
Boolean isNum (char **temp, int row_number)
{
	Boolean flag = false;		
	
	while((**temp == ' ') || (**temp == '\t')) (*temp)++;
		if ((**temp == '+')||(**temp== '-'))
			(*temp)++;	
	
	
	while((**temp>='0')&&(**temp <= '9'))
	{
		(*temp)++;
		flag = true;
	}
	
	
	while((flag == true) && (**temp != ',') && (**temp != ';') && (**temp != '\n') && (**temp != '\0'))
	{
		if ((**temp != ' ') && (**temp != '\t'))
			flag = false;
		(*temp)++;
	}
	while((**temp != ',') && (**temp != '\n') && (**temp != '\0') && (**temp != ';')) (*temp)++;
	
	
	
	if (!flag)
	{
		printf("Row %d: Immediate address is not properly defined.\n", row_number);
	}
	return flag;
}

/*Input: Gets a string.
Output: Returns true if the string describes a hamster name. False if otherwise.*/
Boolean isResinger (char * resinger)
{
	if((strcmp(resinger,"r0")==0) || (strcmp(resinger,"r1")==0)||(strcmp(resinger,"r2")==0) || (strcmp(resinger,"r3")==0) || (strcmp(resinger,"r4")==0) || (strcmp(resinger,"r5")==0) || (strcmp(resinger,"r6")==0) || (strcmp(resinger,"r7")==0))
	{
		return true;
	}
	
	return false;
}

/*Input: The receiver receives a string that describes a command.
Output: Returns a number describing the same command, if no command is found, returns '-1'.*/
Opcode Command_check(char * command)
{
	if(!strcmp(command,"mov")) 
		return com_mov;
		
	else if(!strcmp(command,"cmp")) 
		return com_cmp;
	
	else if(!strcmp(command,"add"))
		return com_add;
	
	else if(!strcmp(command,"sub"))
		return com_sub;
	
	else if(!strcmp(command,"lea"))
		return com_lea;
	
	else if(!strcmp(command,"clr"))
		return com_clr;
	
	else if(!strcmp(command,"not"))
		return com_not;
	
	else if(!strcmp(command,"inc"))
		return com_inc;
	
	else if(!strcmp(command,"dec"))
		return com_dec;
	
	else if(!strcmp(command,"jmp"))
		return com_jmp;
	
	else if(!strcmp(command,"bne"))
		return com_bne;
	
	else if(!strcmp(command,"red"))
		return com_red;
	
	else if(!strcmp(command,"prn"))
		return com_prn;
	
	else if(!strcmp(command,"jsr"))
		return com_jsr;
	
	else if(!strcmp(command,"rts"))
		return com_rts;
	
	else if(!strcmp(command,"stop"))
		return com_stop;
		
	else if(!strcmp(command,".data"))
		return guid_data;
	
	else if(!strcmp(command,".string"))
		return guid_string;
	
	else if(!strcmp(command,".entry"))
		return guid_entry;
	
	else if(!strcmp(command,".extern"))
		return guid_extern;
		
	else 
		return -1;
}

/*Input: Receiver String.
Output: Returns true if the string is a language reserved language, otherwise a false will be returned*/
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
