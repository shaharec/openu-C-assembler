/*Prototypes*/
boolean syntax_chack (char *buff,int row_number); /*Chacking if the file is defind correctly.*/

/*Auxiliary functions*/
void Next_First_Char(char ** first_char,int * comma_count); /*Advance the pointer to the beginning of the wordand update the comma counter.*/
void Next_last_Char(char ** last_char);/*Advance the pointer to the end of the word .*/
boolean address_check(char **first_char, char **last_char,int check_num ,int op_num, int *comma_count, int row_number); /*Verify if correct input for the operator.*/
boolean to_many_arg(char **temp, int row_number); /*Chaking if ther is more characters unti the end of the line.*/
boolean def_lable (char ** first_char, char ** last_char, char* command, int * comma_count, int* colon_count,  int row_number); /*Chacking if lable is proprtly define.*/
boolean islable (char **first_char,char **last_char, int row_number); /* Chacking true if the received word is correct lable.*/
boolean isString (char **first_char, char **last_char, int *comma_count, int row_number); /*Chacking if the received row sets a valid string.*/
boolean next_num (char **first_char, char **last_char,int * comma_count, int row_number); /*Chacking if the received row is set correctly as a row of numbers separated by commas.*/
boolean isNum (char **temp, int row_number); /*Chacking if the there is a properly defind number .*/
boolean isResinger (char * resinger);/*Chacking if the string describes a register. */
cmdType Command_check(char * command); /*Returns a number describing a action word. if no action word is found, returns '-1'.*/
void zero_str (char * str, int len); /*Reset recived string.*/
