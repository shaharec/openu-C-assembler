/*specify data sturcurs for the assembler*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#define LB_NAME_SIZE 31
#define LINE_LEN 81

typedef enum boolean{/*define type of boolean*/
false,true}boolean;

typedef enum lbType{/*label types*/
DATA_LABEL,INST_LABEL,EX_LABEL}lbType;

typedef struct labelAd{/*store the labels and address*/
	char label[LB_NAME_SIZE];/*name of the label*/
	int address;/*address of the label*/
	boolean directive;/*if the label attach to directive sentence 1 else 0*/
	boolean external;/*if the label is external  1, else 0*/
	lbType labelType;/*type of lable*/
}labelAd;

typedef struct lineInfo{/*stores every line information */
	int index;
	char text[LINE_LEN];
	char label[LB_NAME_SIZE];
	char cmd[LINE_LEN];
	lbType type;
}lineInfo;

typedef struct labelTable{/*tbale of label*/
	int size;
	labelAd* labelAd;
}labelTable;

boolean addLb(char* label, lbType labelType);/*add label to label table*/


