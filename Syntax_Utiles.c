#include "DataStruct.h"
#include "Syntax.h"

/*Auxiliary functions for Syntax.c file*/

/*Input: Pointer for beginning word and comma count.
Output: will advance the pointer to the beginning of the word, return true if there is ';' In the section, will return false if not. If a comma encounter , it will update the comma counter*/
void Next_First_Char(char **first_char, int *comma_count)
{	
	/*Counts if there is a comma*/
	if(**first_char==',')
    	{
    		(*comma_count)++;
    		(*first_char)++;
    	}
    	
    	/*Loop runs on white characters and stops if you meet non-white or end-of-line character.*/		
	while((**first_char=='\t' || **first_char==' ' || **first_char==',') && **first_char!=';' && **first_char!='\n' && **first_char!='\0')
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
	/*if (**last_char==',')
		(*last_char)++;*/
	
	/*Loop runs on white characters and stops if you meet non-white or end-of-line character.*/	
	while(**last_char!='\t' && **last_char!=' ' && **last_char!=',' && **last_char!=';' && **last_char!='\n' && **last_char!='\0')
    		(*last_char)++;
    		
    	/* included to get the word that was inputed last properly*/
    	if(**last_char=='\0') 
    		(*last_char)--;  
}


/*Input: The first and last character for the operator word,  and number of checks to be able to do, how many operators the command have, comma counter and the number of the row.
Output: Verify if correct input for the operator, false if incorrect. Promotes pointer values.*/		    	
boolean address_check(char **first_char, char **last_char,int check_num ,int op_num, int *comma_count, int row_number)
{
	int len, comm_checker, op_position;
	char *operator = NULL;
	boolean flag = false;
	
	/*Promotions indicate the beginning of the first character of the word and the last letter of the word. */
	*first_char=*last_char;		
    	Next_First_Char(first_char, comma_count);
    	*last_char=*first_char;
    	Next_last_Char(last_char);
	
	/*Check if the argument is the first or only operator.*/ 
	if (op_num == ONLY ||op_num == FIRST)
		op_position=FIRST;
	/*Check if the argument is the second operator.*/	  
	if (op_num == SECOND)
		op_position=SECOND;
	 
	 /*Check if we have reached the end of the line or a semicolon.*/	  
	if (**first_char == ';' ||**first_char == '\n' || **first_char == '\0')
   	{
  		fprintf(stdout,"Row %d: Missing argument.\n", row_number);
    		return false;
    	}
	  
	
	/*If a comma is missing, an error message is printed and a false is returned.*/				
    	if(*comma_count<op_position) /*check if there is comma and stops if there is*/ 
	{
   		fprintf(stdout,"Row %d: Missing comma.\n", row_number);
    		return false;  		
	}
	
	/*Check for excess comma error message and return false.*/
	else if(*comma_count>op_position) 
	{
		if (op_num==FIRST)/*Check if the argument is the first or only operator.*/
			fprintf(stdout,"Row %d: Missing argument.\n", row_number);
		
		else /*Check if the argument is the second operator and return false.*/
  			fprintf(stdout,"Row %d: Illegal comma.\n", row_number);
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
    			if (**first_char == ';' || **first_char == '\n' || **first_char == '\0')
   			{
  				fprintf(stdout,"Row %d; Argument %d: Missing argument.\n", row_number, op_position+1);
    				return false;
    			}
    			
    		}
    		
    		/*Check if this is an immediate address.*/				    	
	    	if (**first_char=='#')  
	    	{
			(*first_char)++; 
			*last_char=*first_char;			
					
			if (**last_char == '-')/*Check if the number is negative*/		
				(*last_char)++;		
			
			/*Passing the pointer over the number to the end*/
			while (**last_char>='0' && **last_char <= '9') 
 			{
    				(*last_char)++;
       				flag = true;
    			}	
    			
    			/*Checking whether we have reached the end of the line or a semicolon and an error message is returned.*/
    			while(**last_char!=';' && **last_char!='\n' && **last_char!='\0' && **last_char!=',')
    			{
    				/*If it is not a white character or number we will update the flag.*/
    				if ((**last_char<'0' || **last_char > '9') && **last_char!='\t' && **last_char!=' ')
    					flag = false;
    				
    				(*last_char)++;
    			}
    			
    			if (check_num!=fourAddMet)
    			{
    				fprintf(stdout,"Row %d; Argument %d: The argument type is not supported for this command.\n", row_number, op_position+1);
    				Next_last_Char(last_char);/*Promote the last character to the next white character or comma or semicolon or sentence end.*/
    				
    				return false;
    			}
    			
    			/*Chack if the Immediate address is properly defined*/
    			if (flag == false) 
    			{
    				fprintf(stdout,"Row %d; Argument %d: Immediate address is not properly defined.\n", row_number, op_position+1);
    				Next_last_Char(last_char);/*Promote the last character to the next white character or comma or semicolon or sentence end.*/
    				
    				return false;
    			}
    	
    			/*If the operator is set correctly, the truth will be returned.*/
    			else 
				return true;
		}
			    	
		/*Check if this is an indirect register addressing.*/	
		else if ((**first_char=='*') ) 
		{	
	    		(*first_char)++; 
	    		
	    		/*Finding the word length and allocate a dynamic memory for a variable.*/
	    		len=*last_char-*first_char;
	    		operator = (char *)malloc((sizeof(char))*len);
	    		
	    		/*Check if the allocation was done correctly.*/
	    		if (operator == NULL)
	    		{
	    			fprintf(stdout,"Allocation error.\n");
	    			exit(1);
	    		}
	    		
	    		/*Set a "operator "string valiue to zero.*/
	    		zero_str (operator,  sizeof(operator));
	    		
	    		/*Saving a value in a dynamic memory variable.*/
			strncpy(operator, *first_char, len);
			 
			/*Check if the value is Register if no appropriate error message is printed and false returned.*/  	
			if (check_num!=TowAddMet && check_num!=ThreeAddMet && check_num!=fourAddMet)
    			{
    				fprintf(stdout,"Row %d; Argument %d: The argument type is not supported for this command.\n", row_number, op_position+1);
    				
    				free(operator);
    				return false;
    			}
			
			if (isResinger(operator) == false) 
			{
			    	fprintf(stdout,"Row %d; Argument %d: Indirect register addressing is not properly defined.\n", row_number, op_position+1);
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
			fprintf(stdout,"Allocation error.\n");
    			exit(1);
	    	}
		
		/*Set a "operator "string valiue to zero.*/
		zero_str (operator,  sizeof(operator));
		
		/*Saving a value in a dynamic memory variable.*/
		strncpy(operator, *first_char, len);
		
		
		/*Check if this is a direct register addressing.*/
		if (isResinger(operator) == true)
		{
			/*Check if the value is Register if no appropriate error message is printed and false returned.*/  	
			if (check_num!=ThreeAddMet && check_num!=fourAddMet)
    			{
    				fprintf(stdout,"Row %d; Argument %d: The argument type is not supported for this command.\n", row_number, op_position+1);
    				Next_last_Char(last_char);/*Promote the last character to the next white character or comma or semicolon or sentence end.*/
    				
    				return false;
    			}
			
			free(operator);	
			return true;
		}
		
		/*Saving a number representing a operator. Performed using the function "Command_check"*/
		comm_checker=Command_check(operator);	
		
		/*Check if the second word is a reserved word*/
		if (comm_checker>= guid_data && comm_checker <= guid_extern) 
		{
			fprintf(stdout,"Row %d; Argument %d: The operator is a reserved word.\n", row_number, op_position+1);
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
		fprintf(stdout,"Row %d; Argument %d: The operand is not properly configured.\n", row_number, op_position+1);
		free(operator);
		return false;
	   			
	
	}
}    	


/*Input: Pointer to end of word and row number.
Output: Moving on the characters to the end of the line and  return true if they are white spaces or a note. else false.*/
boolean to_many_arg(char **temp, int row_number)
{
	boolean flag=false;
	int comma_counter=0;
	/*Loop for passing on white charecters*/
 	while(**temp!=';' && **temp!='\n' && **temp!='\0')					
   	{
   		if (**temp==',')
   			comma_counter++;
   			
   		if (**temp!=' ' && **temp!='\t' && **temp!=',')
   			flag = true;
   			
   		(*temp)++;
   	}
   	if (flag == false && comma_counter==0)
   		return true;
      	
      	/*Print error values in case of an invalid character that came before the end of the line. return false*/
      	
      	else if (comma_counter>0)
 		fprintf(stdout,"Row %d: Illegal comma.\n", row_number);
      	
      	if (flag == true)	
      		fprintf (stdout,"Row: %d: To many arguments.\n", row_number);
      
      	
		
	return false;
		
}
	
/*Input: Gets Pointer to the beginning and end of a word, comma counter, colon counter and row number.
Output: Returns true if the first word in the line is a label, false if otherwise. Will update the counters if necessary and save the first word value in a row in the "command" variable. Promotes pointer values.*/
boolean def_lable (char ** first_char, char ** last_char, char* command, int * comma_count, int* colon_count,  int row_number)
{
	int len=0, comm_checker;
	boolean correct_lable = false;
	char * temp;
		
	/*Check if the argument is a valid label and update the flag.*/
	correct_lable = islable (first_char, last_char, row_number);
	
	/*If the word is not a label, we will move the pointer to the last character at the end of the word.*/
	if (correct_lable == false)
	{
		*last_char=*first_char;
		Next_last_Char(last_char);
	}
		
    	/*Placing a string in the "command" array.*/
    	len=*last_char-*first_char;
    	strncpy(command, *first_char, len);
    	
    	/*Saving a number representing a command. Performed using the function "Command_check"*/
    	comm_checker=Command_check(command);
    
    	
    	
    	/*Check if the "command" string is a language-defined executable. Returns false without error message*/
    	
    	/*Check if the "command" string is a language-defined executable. Returns false */
    	if ((comm_checker>= com_mov) && (comm_checker <= guid_extern))
    	{
    		/*Prints an error if set to label*/
    		if (**last_char ==':' && (*colon_count==1)) 
    			fprintf(stdout,"Row %d: Laibel is a reserv word.\n", row_number);

    		return false;
    	}
    	
    	if (correct_lable==true)
    	{
    		temp=*last_char;
    		
	    	if (**last_char == ':')
	    	{
	    		temp++; /*.*/
	    		if (*temp!='\t' && *temp!=' ' && *temp!=';' &&  *temp!='\n' && *temp!='\0')
	    		{
	    			correct_lable=false; 
	    			Next_last_Char(&temp);
	    		}
	    	}
	    		
	    	else
	    	{
	    		correct_lable=false;
	    		Next_last_Char(&temp);
	    	}	
    		
    		*last_char = temp;
    	}
    	
    	
    	/*Check if a label is set up correctly.*/
	if(correct_lable==true)
    			return true;
    	
    	/*Check if a label is not set up correctly. return false and prints error.*/
    	else
    	{
    		fprintf(stdout,"Row %d: Laibel is not properly define.\n", row_number);
    		return false;
    	}
}

/*Input: Gets Pointer to the beginning and end of a word and row number.
Output: Returns true if the received word is correct in terms of the lable syntax. False if not*/
boolean islable (char **first_char,char **last_char, int row_number)
{
	*last_char = *first_char; /*Update pointer to last character to pointer to first character.*/
	
	/*An error will be returned if a tag starts with '.'*/
	if (**last_char == '.')
		return false;
	
	/*Enter the loop only if the word consists of letters or numbers*/	
	while (((**last_char>='0' && **last_char<='9') || (**last_char>='A' && **last_char<='Z') || (**last_char>='a' && **last_char<='z')) && **last_char!=';' && **last_char!='\n' && **last_char!='\0')
	{	
		(*last_char)++;
		
		/*An error message will be returned if the label length is longer than allowed.*/
		if ((*last_char - *first_char) > LB_NAME_SIZE)
		{
			fprintf (stdout,"Row: %d: Invalid label.\n", row_number);
			return false;
		}
	}
	
	return true;
}

/*Input: Gets Pointer to start and end of argument, comma counter and row number.
Output: Returns true if the received row sets a valid string. False if not. Promotes pointer values*/
boolean isString (char **first_char, char **last_char, int *comma_count, int row_number)
{
	int quote_count=0;
	boolean flag = true;
	
	/*We will move the pointer to the beginning of a word and point to the end of a word for the begining of the next word*/
	*first_char = *last_char;
	Next_First_Char(first_char, comma_count);
	*last_char=*first_char;
	
	
	if (**last_char == '\"') 
	{	
		/*Check if we have reached the end of a line.*/
		while(**last_char != '\n' && **last_char != '\0')
		{	
			if (**last_char == '\"')
				quote_count++;/*Update the quotation mark counter.*/
		
			(*last_char)++;
		}
		
		/*Entered the loop as long as we didn't get a quotation mark*/
		while(**last_char != '\"')
		{
			(*last_char)--;
			
			/*If it is a non-white or quotation mark, we will update the flag to display an error.*/
			if (**last_char != '\"' && **last_char != '\t' && **last_char != ' ')
				flag = false;
			
			/*If it is a semicolon, we will update the flag to display true*/	
			if(**last_char == ';')
				flag = true;	
		}
		
		/*If there are at least 2 quotation marks and after the string no further arguments will be returned true.*/
		if (quote_count>=2 && flag == true)
			return true;
		
		/*If there are less than 2 incorrect quotation marks the error message is returned.*/
		else if (quote_count<2)
			fprintf(stdout,"Row %d: Illegal sting, missing quotation marks.\n", row_number);
	    	
	    	/*If there are other arguments that are not set to the string, an error message will be returned.*/	
		else if (quote_count>=2 && flag == false)
			fprintf(stdout,"Row %d: Too many arguments.\n", row_number);
		
		return false;
	}
	
	/*If no quotation mark appears, an error message will be returned.*/	
	else	
	{
		fprintf(stdout,"Row %d: Expected to string.\n", row_number);
 		return false;
 	}
}

/*Input: Gets Pointer to the beginning and end of an argument, comma counter and row number.
Output: Returns true if the received row is set correctly as a row of numbers separated by commas. Otherwise return false. Promotes pointer values*/
boolean next_num (char **first_char, char **last_char,int * comma_count, int row_number)
{
	boolean flag = true;
	int num_count=0;

	/*We will move the pointer to the beginning of a word and point to the end of a word for the begining of the next word*/
	*first_char=*last_char;
	Next_First_Char(first_char,comma_count); /*Skip white characters*/
	*last_char=*first_char;
	
	/*If we reach the end of a line or a semicolon, a false will return and an error message will be printed.*/
	if (**last_char == ';' || **last_char == '\n' || **last_char == '\0')
	{
		fprintf(stdout,"Row %d: Number missing.\n", row_number);
    		return false;
	}
	
	/*Enter the loop as long as we have not reached the end of a line or a semicolon.*/
	while(**last_char != ';' && **last_char != '\n' && **last_char != '\0') 
	{	
		
		/*Increases the valid number counter if a valid number is specified.*/
		if (isNum (last_char, row_number) == true)
			num_count++;
		
		else
			if (flag == true) flag = false; /*Otherwise, it will update the flag to display an error.*/
		
		/*If you encounter a comma, it will update the comma counter.*/
		if (**last_char == ',')
		{
			(*comma_count)++;
			(*last_char)++;	
		}
		
		*first_char=*last_char; /*Update the first character pointer to indicate the last character place.*/
	}
	
	
	if (flag == true)/*Check if no well-defined numbers appear in the row*/
	{
		/*If there is one comma less than the number of numbers in a row, a true value will be returned.*/
		if (num_count == *comma_count + 1)
			return true;
		
		/*If there are more stops the numbers in the row a false value returns an error message.*/
		else if (num_count < *comma_count + 1)
			fprintf(stdout,"Row %d: Illegal comma.\n", row_number);

		/*If there are fewer stops the numbers in the row a false value returns an error message.*/
		else
			fprintf(stdout,"Row %d: Missing comma.\n", row_number);
    		
    		return false;
		
	}
	
	else /*If there is a value other than a valid number, an error message will be printed and a false will be returned.*/
	{
		fprintf(stdout,"Row %d: Immediate address is not properly defined.\n", row_number);
    		return false;
	}		
}

	
/*Input: Gets Pointer to the beginning and end of an argument, comma counter and line number.
Output: Returns true if the argument sent is true number, otherwise false.
Promotes pointer values untill the next comma.*/
boolean isNum (char **temp, int row_number)
{
	boolean flag = false;		
	
	/*Loop for passing on white characters.*/
	while(**temp == ' ' || **temp == '\t') (*temp)++;
	
	/*If meets '+' or '-', you will advance the pointer.*/
	if (**temp == '+' || **temp== '-') (*temp)++;	
	
	/*If a numeric value appears, the flag will update.*/ 
	while(**temp>='0' && **temp <= '9')
	{
		(*temp)++;
		flag = true;
	}
	
	/*We will run until the next comma / semicolon / end line as long as it is a valid number.*/
	while(flag == true && **temp != ',' && **temp != ';' && **temp != '\n' && **temp != '\0')
	{
		/*If we encounter a non-white value, we will update the flag to false.*/
		if (**temp != ' ' && **temp != '\t') flag = false;
		
		(*temp)++;
	}
	
	/*We will run until the next comma / semicolon / end line.*/
	while(**temp != ',' && **temp != ';' && **temp != '\n' && **temp != '\0') (*temp)++;
	
	return flag;
}

/*Input: Gets a string.
Output: Returns true if the string describes a register. False if otherwise.*/
boolean isResinger (char * resinger)
{
	/*A comparison of the string that receives the function with the stored register names to the language if there is a match will be returned true.*/
	if(strcmp(resinger,"r0")==0 || strcmp(resinger,"r1")==0 || strcmp(resinger,"r2")==0 || strcmp(resinger,"r3")==0 || strcmp(resinger,"r4")==0 || strcmp(resinger,"r5")==0 || strcmp(resinger,"r6")==0 || strcmp(resinger,"r7")==0)
	{
		return true;
	}
	
	return false;
}

/*Input: The receiver receives a string that describes a command.
Output: Returns a number describing the same command, if no command is found, returns '-1'.*/
cmdType Command_check(char * command)
{
	/*Comparison of the string that receives the function with the names of the command-type saved languages. If there is a match, a representative numeric value is returned for each command.*/
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


/*Input: String and character length to be reset.
Output: Reset the string.*/
void zero_str (char * str, int len)
{
	int i;/*Index*/
	
	/*A loop that passes through a set of characters and initializes its contents.*/
	for(i=0; i<len; i++)
		str[i]=0;
}

