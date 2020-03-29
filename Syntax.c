#include "Syntax.h"

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
	
	if(strlen(buff)>LINE_LEN-1){/*check line length*/
		fprintf(stdout,"Row %d: max line length is %d the line is bigger\n",row_number,(LINE_LEN-1));
		error = false;
	}
	
	/*Find the first character in the next word.*/
	first_char=buff;
	Next_First_Char(&first_char,&comma_count);
	
	/*A case of a line of white characters or a line that begins with a semicolon.*/
	if (*first_char==';' || *first_char=='\n' || *first_char=='\0')
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
			if ((next_num(&first_char, &last_char,&comma_count, row_number)== false) && (error == true))
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
    				fprintf(stdout,"Row %d: WORNING! '.entry' cannot be labled.\n", row_number);
    			
    			/*Find the first character in the next word.*/			
    			first_char = last_char;
    			Next_First_Char(&first_char,&comma_count);	
    			
    			/*If there was a comma in the first word and prints error if so and update the flag.*/		
    			if(comma_count>0)
    			{
    				fprintf(stdout,"Row %d: Illegal comma.\n", row_number);
    				error = false;
    				
    				/*Find the first character in the next word.*/
    				first_char++;
    				Next_First_Char(&first_char,&comma_count);
    			}
    			
    			/*Check if a semicolon is set before checking the operators then an error message will print and move to the next iteration of the main loop.*/
    			if (*first_char == ';')
    			{
    				fprintf(stdout,"Row %d: Invalid command line.\n", row_number);
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
 				fprintf(stdout,"Row %d: WORNING! '.extern' cannot be labled.\n", row_number);		
    			
    			/*Find the first character in the next word.*/
    			first_char = last_char;
    			Next_First_Char(&first_char,&comma_count);	
    			
    			/*If there was a comma in the first word and prints error if so and update the flag.*/			
    			if(comma_count>0)
    			{
    				fprintf(stdout,"Row %d: Illegal comma.\n", row_number);
    				error = false;
    				
    				first_char++;
    				Next_First_Char(&first_char,&comma_count);
    			}
    			
    			/*Check if a semicolon is set before checking the operators then an error message will print and move to the next iteration of the main loop.*/	
    			if (*first_char == ';')
    			{
    				fprintf(stdout,"Row %d: Invalid command line.\n", row_number);
    				error = false;
    						
    				break;
    			}
    			
    			/*Check if the argument is a valid label and update the flag.*/		
    			if((islable(&first_char, &last_char, row_number)==false) && (error == true))
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

			/*Check if there are additional arguments in the line and update the flag.*/
			if((to_many_arg(&last_char, row_number)== false) && (error == true))
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
    		case com_jsr:/*jsr command*/
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
    			fprintf(stdout,"Row %d: Invalid command line.\n", row_number);
    					error = false;
    						
    			break;	
    		}
    			
	}
	
	/*return if error was found in the file*/
	return error;	
	
}
