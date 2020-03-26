#include "DataStruct.h"
#include "syntax.h"

/*Function*/
/*Input: Receives a reading line
Output: Return true if the file is correct and return false and print the error essence if the input is incorrect.*/
boolean syntax_chack (char *buff, int row_number)
{	
	char *first_char=NULL, *last_char=NULL;		/*Pointer to the first character and pointer to the last character in the word.*/
	char command [MAX_COMMAND] = {0};		/*String to hold action words.*/
	cmdType comm_checker; 				/*A variable that holds a number represents an action word.*/
	int comma_count=0, colon_count=0;		/*Comma and doubel dots counters.*/
	int len=0;					/*A numeric variable that holds the length of each word.*/
	boolean error = true, labled = false;		/*Flags for saving error and label appearances.*/
	
	printf("%d.%s", row_number, buff);
	
	/*Find the first character in the next word.*/
	first_char=buff;
	Next_First_Char(&first_char,&comma_count);
	
	/*A case of a line of white characters or a line that begins with a semicolon.*/
	if (*first_char=='\n'|| *first_char=='\0' || *first_char==';') 
		return true;

	/*Check if a label is set, if so the instruction word is recorded to the "Command" argument . If you do not receive the instruction, the argument is "Command"*/
    	if (def_lable (&first_char,&last_char,command, &comma_count, &colon_count,row_number) ==true)
    	{
    		labled = true; /*There is a label*/
    		
    		/*Find the next word.*/
    		first_char = last_char;
    		Next_First_Char(&first_char,&comma_count); 	
    		last_char=first_char;
    		Next_last_Char(&last_char);
    		
    		zero_str (command, MAX_COMMAND); /*Resetting the "command" argument array*/
    		
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
			if ((next_num (&first_char, &last_char,&comma_count, row_number)== false) && (error == true))
    				error = false;
    			
    			/*Check if there are additional arguments in the line and update the flag.*/
    			/*if ((to_many_arg(&last_char, row_number)== false) && (error == true))
    				error = false;*/
    						
    			break;
    		}
    			
    		case guid_string:/*.string command*/
    		{
			/*Check if the string is set correctly for the directive sentence.*/
    			if((isString(&first_char , &last_char, &comma_count ,row_number )== false) && (error == true))
    				error = false;
    					
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
    				error = false;
    				
    				/*Find the first character in the next word.*/
    				first_char++;
    				Next_First_Char(&first_char,&comma_count);
    			}
    			
    			/*Check if a semicolon is set before checking the operators then an error message will print and move to the next iteration of the main loop.*/
    			if (*first_char == ';')
    			{
    				printf("Row %d: Invalid command line.\n", row_number);
    				error = false;
    						
    				break;
    			}
    		
    			/*Check if the argument is a valid label and update the flag.*/			
    			if((islable (&first_char, &last_char, row_number)==false) && (error == true))
    				error = false;
    			
    			/*Check if there are additional arguments in the line and update the flag.*/				
    			if((to_many_arg(&last_char, row_number)== false) && (error == true))
    				error = false;
    						
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
    				error = false;
    				
    				first_char++;
    				Next_First_Char(&first_char,&comma_count);
    			}
    			
    			/*Check if a semicolon is set before checking the operators then an error message will print and move to the next iteration of the main loop.*/	
    			if (*first_char == ';')
    			{
    				printf("Row %d: Invalid command line.\n", row_number);
    				error = false;
    						
    				break;
    			}
    			
    			/*Check if the argument is a valid label and update the flag.*/		
    			if((islable (&first_char, &last_char, row_number)==false) && (error == true))
    				error = false;
    			
    			/*Check if there are additional arguments in the line and update the flag.*/				
    			if((to_many_arg(&last_char, row_number)== false) && (error == true))
    				error = false;
    						
    			break;	
    					
       		}
    			
    		/*4 source operand, 3 Target operand.*/
    		case com_mov: /*mov command*/
    		case com_add: /*add command*/
    		case com_sub: /*sub command*/
    		{
    			/*Checking the first argument and updating the flag.*/
     			if ((address_check(&first_char, &last_char, fourAddMet, FIRST, &comma_count, row_number)== false) && (error == true)) 
    				error = false;
				
			/*Checking the second argument and updating the flag.*/		
    			if ((address_check(&first_char, &last_char, ThreeAddMet, SECOND, &comma_count, row_number) == false) && (error == true)) 
    				error = false;
			
			/*Check if there are additional arguments in the line and update the flag.*/
    			if((to_many_arg(&last_char, row_number)== false) && (error == true))
    				error = false;
    				
    			break;
    		} 
    			
    		/*4 source operand, 4 Target operand.*/
    		case com_cmp:/*cmp command*/
    		{
    			/*Checking the first argument and updating the flag.*/
    			if ((address_check(&first_char, &last_char, fourAddMet, FIRST, &comma_count, row_number) == false) && (error == true))
    				error = false;    	
    			
    			/*Checking the second argument and updating the flag.*/				
    			if ((address_check(&first_char, &last_char, fourAddMet, SECOND, &comma_count, row_number) == false) && (error == true)) 
    				error = false;

    			break;
    		}
    			
    		/*1 source operand, 3 Target operand.*/
    		case com_lea:
    		{
    			/*Checking the first argument and updating the flag.*/
    			if ((address_check(&first_char, &last_char, OneAddMet, FIRST, &comma_count, row_number) == false) && (error == true)) 
    				error = false;
    				
    			/*Checking the second argument and updating the flag.*/	
    			if ((address_check(&first_char, &last_char, ThreeAddMet, SECOND, &comma_count, row_number) == false) && (error == true)) 
    				error = false;
    			
    			/*Check if there are additional arguments in the line and update the flag.*/	
    			if((to_many_arg(&last_char, row_number)== false) && (error == true))
    				error = false;
    					
    			break;
    		}
    			
    		/*No source operand, 2 Target operand.*/
    		case com_jmp:/*jmp command*/
    		case com_bne:/*ben command*/
    		{
    			/*Test the only argument and update the flag.*/
			if ((address_check(&first_char, &last_char, TowAddMet, ONLY,&comma_count, row_number) == false) && (error == true)) 
    				error = false;
    			
    			/*Check if there are additional arguments in the line and update the flag.*/	
    			if((to_many_arg(&last_char, row_number)== false) && (error == true))
    				error = false;
    					
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
			if ((address_check(&first_char, &last_char, ThreeAddMet, ONLY, &comma_count, row_number) == false) && (error == true)) 
    				error = false;
    			
    			/*Check if there are additional arguments in the line and update the flag.*/
    			if((to_many_arg(&last_char, row_number)== false) && (error == true))
    				error = false;
    			break;
    		}
    			
    		/*No source operand, 4 Target operand.*/
    		case com_prn:/*prn command*/
    		{
    			/*Test the only argument and update the flag.*/
			if ((address_check(&first_char, &last_char, fourAddMet, ONLY, &comma_count, row_number) == false) && (error == true)) 
    				error = false;
    			
    			/*Check if there are additional arguments in the line and update the flag.*/	
    			if((to_many_arg(&last_char, row_number)== false) && (error == true))
    				error = false;
    				
    			break;
    		}
    			
    		/*No source operand, no target operand.*/
    		case com_rts:/*rts command*/
    		case com_stop:/*stop command*/
    		{
    			/*Check if there are additional arguments in the line and update the flag.*/
    			if((to_many_arg(&last_char, row_number)== false) && (error == true))
    				error = false;
    					
    			break;
    		}
    		
    		/*If command is no specified*/
    		default:
    		{
    			printf("Row %d: Invalid command line.\n", row_number);
    					error = false;
    						
    			break;	
    		}
    			
	}
	
	return error;	
	
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
	    		operator = (char *)malloc(len);
	    		
	    		/*Check if the allocation was done correctly.*/
	    		if (operator == NULL)
	    		{
	    			printf("Allocation error.\n");
	    			exit(1);
	    		}
	    		
	    		/*Set a "operator "string valiue to zero.*/
	    		zero_str (operator,  sizeof(operator));
	    		
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
		
		/*Set a "operator "string valiue to zero.*/
		zero_str (operator,  sizeof(operator));
		
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
boolean to_many_arg(char **temp, int row_number)
{
	/*Loop for passing on white charecters*/
 	while((**temp)=='\t' || (**temp)==' ')					
   		(*temp)++;
   	
   	/*If the pointer reaches the end of a line or a semicolon, return true .*/		
	if((**temp)==';' ||(**temp)=='\n' || (**temp)=='\0')
      		return true;
      	
      	/*Print error values in case of an invalid character that came before the end of the line. return false*/
      	else if ((**temp)==',')
 		printf("Row %d: Illegal comma.\n", row_number);
		
      	else	
      		printf ("Row: %d: To many arguments.\n", row_number);
	
	return false;
		
}
	
/*Input: Gets Pointer to the beginning and end of a word, comma counter, colon counter and row number.
Output: Returns true if the first word in the line is a label, false if otherwise. Will update the counters if necessary and save the first word value in a row in the "command" variable. Promotes pointer values.*/
boolean def_lable (char ** first_char, char ** last_char, char* command, int * comma_count, int* colon_count,  int row_number)
{
	int len=0, comm_checker;
	boolean correct_lable = false;
	char *temp=NULL;
	
	/*Check if the argument is a valid label and update the flag.*/
	correct_lable = islable (first_char, last_char, row_number);
	
	/*If the word is not a label, we will move the pointer to the last character at the end of the word.*/
	if (correct_lable == false)
		Next_last_Char(last_char);
		
	temp = *last_char; /*We will temporarily save the pointer value to the last character.*/
	
	/*We will promote the temporary pointer to run across the line to find a non-white character.*/
	while((*temp=='\t' || *temp==' ') && *temp!=':' &&  *temp!='\n' && *temp!='\0')
    		temp++;
    	
    		
    	if(*temp==':') /*Check to see if the character we are standing on with the pointer is a dots that describes a valid tag definition*/
    	{
    		temp++;	
    		(*colon_count)++;
    	}
    	
    	/*Placing a string in the "command" array.*/
    	len=*last_char-*first_char;
    	strncpy(command, *first_char, len);
    	
    	/*Saving a number representing a command. Performed using the function "Command_check"*/
    	comm_checker=Command_check(command);
    	
    	
    	*last_char = temp; /*Update the pointer that points to the last character of the temporary pointer*/
    	
    	/*Check if the "command" string is a language-defined executable. Returns false without error message*/
    	
    	/*Check if the "command" string is a language-defined executable. Returns false */
    	if ((comm_checker>= com_mov) && (comm_checker <= guid_extern))
    	{
    		/*Prints an error if set to label*/
    		if ((*((*last_char)-1) ==':') && (*colon_count==1)) 
    			printf("Row %d: Laibel is a reserv word.\n", row_number);

    		return false;
    	}
    	
    	
    	/*Check if a label is set up correctly.*/
	if(correct_lable==true)
    	{	
    		/*Returns true if set to label properly.*/
    		if ((*((*last_char)-1) ==':') && (*colon_count==1))
    			return true;
        	
        	/*Prints an error if  not set to label properly.*/	
    		else
    		{
    			printf("Row %d: Laibel is not properly define.\n", row_number);
    			return false;
		}
    	}
    	
    	/*Check if a label is not set up correctly. return false and prints error.*/
    	else
    	{
    		printf("Row %d: Laibel is not properly define.\n", row_number);
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
	while ((((**last_char)>='0' && (**last_char)<='9') || ((**last_char)>='A' && (**last_char)<='Z') || ((**last_char)>='a' && (**last_char)<='z')) && (**last_char!=';') && (**last_char!='\n') && (**last_char!='\0'))
	{	
		(*last_char)++;
		
		/*An error message will be returned if the label length is longer than allowed.*/
		if ((*last_char - *first_char) > LB_NAME_SIZE)
		{
			printf ("Row: %d: Invalid label.\n", row_number);
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
	
	
	if ((**last_char) == '\"') 
	{	
		/*Check if we have reached the end of a line.*/
		while((**last_char != '\n') && (**last_char != '\0'))
		{	
			if ((**last_char) == '\"')
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
		if ((quote_count>=2) && (flag == true))
			return true;
		
		/*If there are less than 2 incorrect quotation marks the error message is returned.*/
		else if (quote_count<2)
			printf("Row %d: Illegal sting, missing quotation marks.\n", row_number);
	    	
	    	/*If there are other arguments that are not set to the string, an error message will be returned.*/	
		else if ((quote_count>=2) && (flag == false))
			printf("Row %d: Too many arguments.\n", row_number);
		
		return false;
	}
	
	/*If no quotation mark appears, an error message will be returned.*/	
	else	
	{
		printf("Row %d: Expected to string.\n", row_number);
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
		printf("Row %d: Number missing.\n", row_number);
    		return false;
	}
	
	/*Enter the loop as long as we have not reached the end of a line or a semicolon.*/
	while((**last_char != ';') && (**last_char != '\n') && (**last_char != '\0')) 
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
		if ((num_count == *comma_count + 1))
			return true;
		
		/*If there are more stops the numbers in the row a false value returns an error message.*/
		else if (num_count < *comma_count + 1)
			printf("Row %d: Illegal comma.\n", row_number);

		/*If there are fewer stops the numbers in the row a false value returns an error message.*/
		else
			printf("Row %d: Missing comma.\n", row_number);
    		
    		return false;
		
	}
	
	else /*If there is a value other than a valid number, an error message will be printed and a false will be returned.*/
	{
		printf("Row %d: Illegal symbol.\n", row_number);
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
	while((**temp == ' ') || (**temp == '\t')) (*temp)++;
	
	/*If meets '+' or '-', you will advance the pointer.*/
	if ((**temp == '+')||(**temp== '-')) (*temp)++;	
	
	/*If a numeric value appears, the flag will update.*/ 
	while((**temp>='0')&&(**temp <= '9'))
	{
		(*temp)++;
		flag = true;
	}
	
	/*We will run until the next comma / semicolon / end line as long as it is a valid number.*/
	while((flag == true) && (**temp != ',') && (**temp != ';') && (**temp != '\n') && (**temp != '\0'))
	{
		/*If we encounter a non-white value, we will update the flag to false.*/
		if ((**temp != ' ') && (**temp != '\t')) flag = false;
		
		(*temp)++;
	}
	
	/*We will run until the next comma / semicolon / end line.*/
	while((**temp != ',') && (**temp != '\n') && (**temp != '\0') && (**temp != ';')) (*temp)++;
	
	
	/*The flag displays a false error message.*/
	if (!flag)
		printf("Row %d: Immediate address is not properly defined.\n", row_number);
	
	return flag;
}

/*Input: Gets a string.
Output: Returns true if the string describes a register. False if otherwise.*/
boolean isResinger (char * resinger)
{
	/*A comparison of the string that receives the function with the stored register names to the language if there is a match will be returned true.*/
	if((strcmp(resinger,"r0")==0) || (strcmp(resinger,"r1")==0)||(strcmp(resinger,"r2")==0) || (strcmp(resinger,"r3")==0) || (strcmp(resinger,"r4")==0) || (strcmp(resinger,"r5")==0) || (strcmp(resinger,"r6")==0) || (strcmp(resinger,"r7")==0))
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
