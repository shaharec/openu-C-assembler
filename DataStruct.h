/*specify data sturcurs for the assembler*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include "Constats.h"

typedef enum boolean{/*define type of boolean*/
false,true}boolean;

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
	int size;
	labelAd* labelAd;
}labelTable;

typedef struct lineWords{/*tbale of label*/
	int size;
	char** str;
}lineWords;

typedef struct RAMword{/*RAM word */
	int address;
	unsigned int data: RAM_WORD_SIZE;
}RAMword;

typedef struct RAM{/*tbale of label*/
	int size;
	RAMword *word;
}RAM;

/*prototypes*/
boolean Islabel(const char *str);/*check if the string is a lable*/	
boolean addLb(char* label, lbType labelType);/*add label to label table*/
int isCmd(char *str);/*check if the string is a command*/
labelAd* labelExsist(char *label);/*check if the lable already exsist in the lable table*/
boolean addToMemory(int data,int address);/*add word to to memory*/
void printLbTable(void);/*print lables table*/
int isReg(char *str);/*check if str is register*/
int getDirectWord(char *num);/*return RAM word of direct value*/
int getLabelWord(labelAd* lable);/*return RAM word of label value*/
int getRegWord(int reg,int offset);/*return RAM word of register value with the offset*/
void printBinary(unsigned x);/*prints binery number*/
void freeMemory();/*free data structers memory*/

extern int IC;
extern int DC;
extern labelTable* labelT;
extern char opcode [OP_SIZE][OP_NAME_SIZE];
extern char reg[REG_NUM][REG_NAME_LEN];
extern RAM *memory;

