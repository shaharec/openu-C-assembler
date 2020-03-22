/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Defines*/
#define MAX_LINE 81
#define MAX_LABLE 31
#define MAX_COMMAND 8
#define FIRST 0
#define SECOND 1
#define ONLY 2

int count=0;
/*Typdefs and structiers*/
typedef enum{false,true}Boolean;
typedef enum{faile=-1, mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, red, prn, jsr, rts, stop}Opcode;

/*Prototypes*/
Boolean prossesAsm(const char* fileName);
Boolean syntax_chack (FILE *fp);
void Next_First_Char(char ** first_char,int * comma_count);
void Next_last_Char(char ** last_char);
Boolean addres_chack(char **first_char, char **last_char,int check_num ,int op_num, int *comma_count, int row_number);
Boolean to_many_arg(char **last_char, int row_number);
Boolean def_lable (char ** first_char, char ** last_char, char* command, int * comma_count, int* colon_count,  int row_number);
Boolean islable (char **first_char,char **last_char, int row_number);
Boolean isString (char **first_char, char **last_char, int *comma_count, int row_number);
Boolean next_num (char **first_char, char **last_char,int * comma_count, int row_number);
Boolean isNum (char **last_char, int row_number);
Boolean isResinger (char * resinger);
Opcode Command_check(char * command);
Boolean isReservWord(char* word,int row_number);
