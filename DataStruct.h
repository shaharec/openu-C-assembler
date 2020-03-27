/*specify data sturcurs for the assembler*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include "Constats.h"
/*macros*/
/*check for allocation error*/
#define allcERR(val){\
	if (val == NULL)\
	{printf("Allocation error.\n");\
	exit(1);}\
	}	

/*Typdefs and structiers*/

typedef enum boolean{/*define type of boolean*/
false,true}boolean;

typedef enum cmdType{com_mov=0, com_cmp, com_add, com_sub, com_lea, com_clr, com_not, com_inc, com_dec, com_jmp, com_bne, com_red, com_prn, com_jsr, com_rts, com_stop, guid_data, guid_string, guid_entry, guid_extern}cmdType; /*Number represents action codes.*/

typedef enum AddMet{OneAddMet = 1, TowAddMet, ThreeAddMet, fourAddMet }AddMet; /*Several types of addressing methods*/

typedef enum lbType{/*label types*/
DATA_LABEL,INST_LABEL,EX_LABEL, ENTRY_LABEL}lbType;

typedef enum lnType{/*line type data, instruction,external*/
DATA,INST}lnType;

typedef enum adrresingType{/*addresing methods*/
A_ONE=1,A_TWO=2,A_THREE =4,A_FOUR=8}adrresingType;

typedef struct labelAd{/*store the labels and address*/
	char label[LB_NAME_SIZE];/*name of the label*/
	int address;/*address of the label*/
	lbType labelType;/*type of lable*/
}labelAd;


typedef struct labelTable{/*tbale of label*/
	int size;/*size of lable table*/
	labelAd* labelAd;/*a pointer to the lable array*/
}labelTable;

typedef struct exCall{/*external label call*/
	int address;/*address where the external lable called*/
	char label[LB_NAME_SIZE];/*name of the lable*/
}exCall;

typedef struct exTable{/*tbale of external label*/
	int size;/*size of the external lable*/
	exCall* exCall;/*a pointer to the array of external lables call*/
}exTable;

typedef struct lineWord{/*tbale of words in line*/
	char *str;/*contain the string of the word*/
	char *end;/*pointer to the end of string in the original line*/
}lineWord;

typedef struct lineWords{/*tbale of words in line*/
	int size;/*number of words in the structer*/
	lineWord *word;/*a pointer to the array of words*/
}lineWords;

typedef struct RAMword{/*RAM word in memory*/
	int address;/*address of RAM word in memory*/
	unsigned int data: RAM_WORD_SIZE;/*data of the RAM word*/
}RAMword;

typedef struct RAM{/*tbale of label*/
	int size;/*number of RAM words in the memory*/
	int instructionC;/*number of instruction words*/
	int dataC;/*number of data words*/
	RAMword *word;
}RAM;
typedef struct entryLine{/*entry line in the entry lable call */
	int address;/*address of the lable from entry address*/
	char label[LB_NAME_SIZE];/*name of label*/
}entryLine;

typedef struct enTable{/*tbale of entry labels*/
	int size;/*size of entry calls*/
	entryLine* line;/*entry table line*/
}enTable;


/*prototypes*/
boolean Islabel(char *str);				/*check if the string is a label*/	
boolean addLb(char* label, lbType labelType);		/*add label to label table*/
int isCmd(char *str);					/*check if the string is a command*/
labelAd* labelExist(char *label);			/*check if the label already exsist in the label table*/
boolean addToMemory(unsigned int data,int *address);	/*add word to to memory*/
void printLbTable(void);				/*print labels table*/
int isReg(char *str);					/*check if str is register*/
int getDirectWord(char *num);				/*return RAM word of direct value*/
int getLabelWord(labelAd* label);			/*return RAM word of label value*/
int getRegWord(int reg,int offset);			/*return RAM word of register value with the offset*/
void printBinary(unsigned x);				/*prints binery number*/
boolean addToExT(char* label,int address);		/*add to exteral calls table*/
boolean addToEnT(char* label);				/*add to enternal table*/
void printExT();					/*prints the external label calls*/
void printEnT();					/*prints the external label calls*/
boolean isStr(char* str);				/*returns if str is an assembler valid string*/
void updateDataLabels(int endIC);			/*update the label table for data addresses*/
void updateRAMCounters();				/*update the number of counters in the memory after the firs pass*/
void freeMemory();					/*free data structers memory*/


extern int IC;
extern int DC;
extern labelTable* labelT;
extern char opcode [OP_SIZE][OP_NAME_SIZE];
extern char reg[REG_NUM][REG_NAME_LEN];
extern RAM *memory;
extern exTable* exT;
extern enTable* enT;

