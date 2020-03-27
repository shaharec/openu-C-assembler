#include "DataStruct.h"

int IC=0;/*instruction counter for memory*/
int DC=0;/*data counter for memory*/

RAM *memory=NULL;/*RAM pointer for memory*/
labelTable* labelT = NULL;/*label tabels*/
exTable* exT=NULL;/*external table for external file*/
enTable* enT=NULL;/*entry table, contain entry calls*/

char reg[REG_NUM][REG_NAME_LEN]={/*registers in the system*/	
					"r0",
					"r1",
					"r2",
					"r3",
					"r4",
					"r5",
					"r6",
					"r7"
				};
			
char opcode[OP_SIZE][OP_NAME_SIZE]={	/*commands in the assembler*/
					"mov",
					"cmp",
					"add",
					"sub",
					"lea",
					"clr",
					"not",
					"inc",
					"dec",
					"jmp",
					"bne",
					"red",
					"prn",
					"jsr",
					"rts",
					"stop"
					};


/*input	:unsigned int data	:data word
output	: boolean value if succsefuly added a word to RAM
the function adds data word in the RAM memory structer*/						
boolean addToMemory(unsigned int data,int *address){
	
	if(memory == NULL){/*check if no word was entered to memory, allocate memory for tabel*/
		
		memory = (RAM*)malloc(sizeof(RAM));
		allcERR(memory);/*check for allocation error*/
		memory->size = 1;
		memory->word = (RAMword*)malloc(sizeof(RAMword));
		allcERR(memory->word);
		memory->word->address = *address;
		memory->word->data = data;
	}else	if(memory->size==RAM_MAX_SIZE){/*check the number of words in the memory*/
			printf("not enogth ram memory: over %d RAM words",RAM_MAX_SIZE);
			return false;
	}else	{/*allocate new words in the memory*/
		memory->size++;
		memory->word = realloc(memory->word,(memory->size)*sizeof(RAMword));
		allcERR(memory->word);/*check for allocation error*/
		(memory->word + memory->size-1)->address = *address;
		(memory->word + memory->size-1)->data =data;
		}
	*address+=1;/*add to memory couner 1(DC or IC)*/	
	/*code for checking the labels and binery representation:*//*
	printf("address:%d  data: %d bin data: ",(memory->word +memory->size-1)->address,data);
	printBinary((memory->word + memory->size-1)->data);
	printf("\n");*/
	return true;
}

/*input	:char* label: label name
	 int address: address where the external label was called
output	:boolean values:successfuly update external tabel.	
the function adds a label to the extrnal calls table by address*/
boolean addToExT(char* label,int address){/*add a label to the extrnal calls table*/
		
	if(exT == NULL){/*check if external table is empty, allocate memory for table and row*/
		
		exT = (exTable*)malloc(sizeof(exTable));
		allcERR(exT);/*check for allocation error*/
		exT->size = 1;
		exT->exCall = (exCall*) malloc(sizeof(exCall));
		allcERR(exT->exCall);/*check for allocation error*/
		exT->exCall->address = address;
		strcpy(exT->exCall->label,label);
	}else	{
		exT->size++;
		exT->exCall = realloc(exT->exCall,(exT->size)*sizeof(exCall));
		allcERR(exT->exCall);
		(exT->exCall + exT->size-1)->address = address;
		strcpy((exT->exCall + exT->size-1)->label,label);
		}	
	
	return true;
}

/*input	:char* label: label name
	 int address: address where the external label was called
output	:boolean values:successfuly update external tabel.	
the function adds a label to the extrnal calls table by address*/
boolean addToEnT(char* label){/*add lable to entry table*/
		
	labelAd* labelAd=labelExist(label);/*find lable address in lable table*/
	if(labelAd==NULL){/*if label doesnt exist*/
		printf("entry label does not exsist: %s\n",label);
		return false;
	}else if(enT == NULL){
		
		enT = (enTable*)malloc(sizeof(enTable));
		allcERR(enT);
		enT->size = 1;
		enT->line = (entryLine*) malloc(sizeof(entryLine));
		allcERR(enT->line);
		enT->line->address = labelAd->address;
		strcpy(enT->line->label,label);
	}else	{
		enT->size++;
		enT->line = realloc(enT->line,(enT->size)*sizeof(entryLine));
		allcERR(enT->line);
		(enT->line + enT->size-1)->address = labelAd->address;
		strcpy((enT->line + enT->size-1)->label,label);
		}	
	
	return true;
}
boolean Islabel(char *str){			
    
    int len = strlen(str),i=0;	  
    if(str && *str && (str[len - 1] == ':')){  
     	if (len - 1 > 30) {/*if label not within 30 chars, if yes it will print error but won't compile but will keep parsing*/
       		printf("more then 30 character for label\n");	 
      	}    
        for(i=0;i<len-2;i++) {/*in a label it should be only digits or letters, else it will print error but will keep parsing*/
            if(!isalpha(*(str + i)) || (*(str + len) <= '9' && *(str + len) >= '0' )){
                printf("invalid label\n");
                return false;
            }
         }
         return true;   
    }
    return false;
}



boolean addLb(char* label, lbType labelType){
   
	if(labelExist(label)!=NULL){
		printf("label exist in table");
      		return false;
    	}if (labelT==NULL) {
		labelT = malloc(sizeof(labelTable));
		allcERR(labelT);
	        labelT->size = 0;
	        labelT->labelAd = malloc(sizeof(labelAd));
	        allcERR(labelT->labelAd);
	}else{
		labelT->labelAd = realloc(labelT->labelAd,((labelT->size) + 1) *sizeof(labelAd));
		allcERR(labelT->labelAd);
    	}
	/*****copy values to new row in the label table******/
	strcpy((labelT->labelAd+(labelT->size))-> label, label);/*copy the name*/
	(labelT->labelAd + (labelT->size))-> labelType = labelType;/*define label type*/
	if (labelType == DATA_LABEL) {              /*if label of type Data*/
		(labelT -> labelAd + (labelT->size))->address = DC; 
	}else if(labelType == INST_LABEL){          /*if label of type instraction*/
		(labelT -> labelAd+(labelT->size))->address = IC; 
	}if (labelType == EX_LABEL) { /*if label of type External*/
		(labelT -> labelAd+(labelT->size))->address = 0; 
	} 
    	labelT->size++;
    	return true;
}

/*input	: char* str : string of opcode
output	: int value of the opcode, if doesnt exist returns -1
the function return the value of command in the assembler if doesnt exist returns -1*/
int isCmd(char *str){
	int i=0;
	for(i=0;i<OP_SIZE;i++)
		if(strcmp(opcode[i],str) == 0)
			return i;
	return -1;/*if didnt find the command return -1*/
		
}

/*input	:char *label : name of label
output	:pointer to label in the lable table
the function returns pointer to label in the label table using the name of the label/
if not found returns NULL*/
labelAd* labelExist(char *label){/*check if the label exsit in table*/
	int i=0;
	if(labelT == NULL)/*check if labelT is null*/
		return NULL;
	for(i=0;i<labelT->size;i++)
		if(strcmp(((labelT->labelAd+i)->label),label) == 0)
			return (labelT->labelAd + i);
	return NULL;
}

/*the function prints the label table
checking function*/
void printLbTable(void){
	int i=0;
	printf("*******label table********\n");
	if(labelT==NULL)
		printf("label table is empty\n");
	else for(i=0;i<labelT->size;i++)
		printf("label: %s address: %d label type: %d\n",(labelT->labelAd + i)->label,(labelT->labelAd + i)->address,(labelT->labelAd + i)->labelType);
}

/*input : char *str
output	: int value : value of a information word of label value word
the function get label and returns the RAM information word */
int isReg(char *str){
	
	int i=0;
	for(i=0;i<REG_NUM;i++){
		if(strcmp(reg[i],str) == 0)
			return i;
		}
	return -1;
}

/*input : int reg : the number of the register
	  int offsett : offset of the reg value in the new information word
output	: int value : value of a information word of register  word
the function get label and returns the RAM information word */
int getRegWord(int reg,int offset){
	
	unsigned int word=0;
	word|= reg<<offset;/*allocate the register in the offset bit place*/
	word|=A_THREE;/*turn on the A bit from ARE*/
	return word;
}

/*input : labelAd* label : label to get information word from
output	: int value : value of a information word of label value word
the function get label and returns the RAM information word */
int getLabelWord(labelAd* label){
	
	unsigned int word=0;
	word|=((label->address)<<EO_ARE);/*label address in left 12 bits*/
	if(label->labelType==EX_LABEL)
		word|=A_ONE;/*turn on the R bit from ARE*/
	else	word|=A_TWO;/*turn on the E bit from ARE*/
	return word;
}

/*input : char* num : a string that contain number
output	: int value : value of a information word of direct value word
the function get string num and returns the RAM information word */
int getDirectWord(char *num){
	
	unsigned int word=0;
	word|=(atoi(num+1)<<EO_ARE);/*move the value to the last 12 bits*/
	word|=A_THREE;/*turn on the A bit from ARE*/
	return word; 
}

/*input	: unsigned x: number x should represente a word in RAM
print binery representation of X number
this function is for checking the program works*/
void printBinary(unsigned x){

    if (x != 1 && x != 0) 
        printBinary(x>>1); 
  
    printf("%d", x & 1); 
} 

/*print external tabel
this function is for checking the program works*/
void printExT(void){
	int i=0;
	printf("*******external table********\n");
	if(exT!=NULL)
		for(i=0;i<exT->size;i++)
			printf("ex label :%s, address called: %d\n",(exT->exCall+i)->label,(exT->exCall+i)->address);
}

/*print entry tabel
this function is for checking the program works*/
void printEnT(void){
	int i=0;
	printf("*******entry table********\n");
	if(enT!=NULL)
		for(i=0;i<enT->size;i++)
			printf("en label :%s, address: %d\n",(enT->line+i)->label,(enT->line+i)->address);
}

/*free all allocated memory of global variables*/
void freeMemory(void){
	/*free RAM*/
	if(memory!=NULL){
		free(memory->word);
		free(memory);
		memory=NULL;
	}
	/*free label table*/
	if(labelT!=NULL){
		free(labelT->labelAd);
		free(labelT);
		labelT=NULL;
	}
	/*free external calls*/	
	if(exT!=NULL){
		free(exT->exCall);
		free(exT);
		exT=NULL;
	}
	/*free enternal labels*/	
	if(enT!=NULL){
		free(enT->line);
		free(enT);
		enT=NULL;
	}
}

/*input	: char* str :string that sould be in the format "*****" when *** are diffrent chares
output	: boolean value if the string is in the right format*/
boolean isStr(char* str){
	if((*str == '\"') && (*(str +strlen(str)-1) == '\"'))/*check if tther is " un the begining of the string and at the end*/
		return true;
	return false;
}

/*input	:int endIC : the address of the last instruction word+1
the fuction update the DATA labels address in the label tabel.
using the endIC address.
*/
void updateDataLabels(int endIC){
	int i=0;
	if(labelT == NULL)/*check if labelT is null*/
		return ;
	for(i=0;i<labelT->size;i++)
		if(((labelT->labelAd+i)->labelType)==DATA_LABEL){
			((labelT->labelAd+i)->address)+=endIC;
			}
}

/*the fuction update the sum of instruction and data words in the RAM memory header
the DC should contain the last address of the memory and the IC should contain the last address 
of instructions*/
void updateRAMCounters(void){
	
	if(memory != NULL){
		memory->instructionC = IC-MEMORY_START;
		memory->dataC = DC-IC;
		printf("instructionC: %d dataC: %d\n",memory->instructionC,memory->dataC);
	}
}


