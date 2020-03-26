
/*Prototypes*/
boolean syntax_chack (char *buff,int row_number);

/*Auxiliary functions*/

/*Input: Pointer for beginning word and comma count.
Output: will advance the pointer to the beginning of the word, return true if there is ';' In the section, will return false if not. If a comma encounter , it will update the comma counter*/
void Next_First_Char(char ** first_char,int * comma_count);

/*Input: Pointer to the end of a word.
Output: Will advance the pointer to the end of the word, stop if it has a white space, ';' Or ','*/
void Next_last_Char(char ** last_char);

/*Input: The first and last character for the operator word,  and number of checks to be able to do, how many operators the command have, comma counter and the number of the row.
Output: Verify if correct input for the operator, false if incorrect. Promotes pointer values.*/
boolean address_check(char **first_char, char **last_char,int check_num ,int op_num, int *comma_count, int row_number);

/*Input: Pointer to end of word and row number.
Output: Moving on the characters to the end of the line and  return true if they are white spaces or a note. else false.*/
boolean to_many_arg(char **temp, int row_number);

/*Input: Gets Pointer to the beginning and end of a word, comma counter, colon counter and row number.
Output: Returns true if the first word in the line is a label, false if otherwise. Will update the counters if necessary and save the first word value in a row in the "command" variable. Promotes pointer values.*/
boolean def_lable (char ** first_char, char ** last_char, char* command, int * comma_count, int* colon_count,  int row_number);

/*Input: Gets Pointer to the beginning and end of a word and row number.
Output: Returns true if the received word is correct in terms of the lable syntax. False if not*/
boolean islable (char **first_char,char **last_char, int row_number);

/*Input: Gets Pointer to start and end of argument, comma counter and row number.
Output: Returns true if the received row sets a valid string. False if not. Promotes pointer values.*/
boolean isString (char **first_char, char **last_char, int *comma_count, int row_number);

/*Input: Gets Pointer to the beginning and end of an argument, comma counter and row number.
Output: Returns true if the received row is set correctly as a row of numbers separated by commas. Otherwise return false. Promotes pointer values*/
boolean next_num (char **first_char, char **last_char,int * comma_count, int row_number);

/*Input: Gets Pointer to the beginning and end of an argument, comma counter and line number.
Output: Returns true if the argument sent is true number, otherwise false.
Promotes pointer values untill the next comma.*/
boolean isNum (char **temp, int row_number);

/*Input: Gets a string.
Output: Returns true if the string describes a hamster name. False if otherwise.*/
boolean isResinger (char * resinger);

/*Input: The receiver receives a string that describes a command.
Output: Returns a number describing the same command, if no command is found, returns '-1'.*/
cmdType Command_check(char * command);

/*Input: String and character length to be reset.
Output: Reset the string.*/
void zero_str (char * str, int len);
