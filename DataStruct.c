#include "DataStruct.h"

int IC=0;/*instruction counter for memory*/
int DC=0;/*data counter for memory*/

RAM *memory=NULL;
labelTable* labelT = NULL;
char reg[REG_NUM][REG_NAME_LEN]={	
					"r0",
					"r1",
					"r2",
					"r3",
					"r4",
					"r5",
					"r6",
					"r7"
				};
			
char opcode[OP_SIZE][OP_NAME_SIZE]={	"mov",
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
boolean addToMemory(int data,int address){
	
	if(memory == NULL){
		
		memory = (RAM*)malloc(sizeof(RAM));
		memory->size = 1;
		memory->word = (RAMword*)malloc(sizeof(RAMword));
		memory->word->address = address;
		memory->word->data = data;
	}else	if(memory->size==RAM_MAX_SIZE){
		printf("not enogth ram memory: over %d RAM words",RAM_MAX_SIZE);
		return false;
	}else	{
		memory->size++;
		memory->word = realloc(memory->word,(memory->size)*sizeof(RAMword));
		(memory->word + memory->size-1)->address = address;
		(memory->word + memory->size-1)->data =data;
		}
	address++;	
	printf("address:%d  data: %d bin data: ",(memory->word +memory->size-1)->address,data);
	printBinary((memory->word + memory->size-1)->data);
	printf("\n");
	return true;
}


boolean Islabel(const char *str){			
    
    int len = strlen(str),i=0;	  
    if(str && *str && (str[len - 1] == ':')){  
     	if (len - 1 > 30) {/*if label not within 30 chars, if yes it will print error but won't compile but will keep parsing*/
       		printf("more then 30 character for label\n");	 
      	}    
        for(i=0;i<len-2;i++) /*in a label it should be only digits or letters, else it will print error but will keep parsing*/
            if(!isalpha(*(str + i)) || (*(str + len) <= '9' && *(str + len) >= '0' )){
                printf("invalid label\n");
                return false;
            }
         return true;   
    }
    return false;
}


boolean addLb(char* label, lbType labelType){
   
	if(labelExsist(label)!=NULL){
		printf("label exist in table");
      		return false;
    	}
	if (labelT==NULL) {
		labelT = malloc(sizeof(labelTable));
	        labelT->size = 0;
	        labelT->labelAd = malloc(sizeof(labelAd));
	}else{
		labelT->labelAd = realloc(labelT->labelAd,((labelT->size) + 1) *sizeof(labelAd));
    	}
	/*****copy values to new row in the label table******/
	strcpy((labelT->labelAd+(labelT->size))-> label, label);/*copy the name*/
	(labelT->labelAd + (labelT->size))-> labelType = labelType;/*define label type*/
	if (labelType == DATA_LABEL) {              /*if label of type Data*/
		(labelT -> labelAd + (labelT->size))->address = DC;
		DC++;    
	}else if(labelType == INST_LABEL){          /*if label of type instraction*/
		(labelT -> labelAd+(labelT->size))->address = IC; 
	}if (labelType == EX_LABEL) {          /*if label of type External*/
		(labelT->labelAd + (labelT->size))->address = 0; 
	} 
    	labelT->size++;
    
    
    	return true;
}

int isCmd(char *str){
	int i=0;
	for(i=0;i<OP_SIZE;i++)
		if(strcmp(opcode[i],str) == 0)
			return i;
	return -1;/*if didnt find the command return -1*/
		
}


labelAd* labelExsist(char *label){/*check if the lable exsit in table*/
	int i=0;
	if(labelT == NULL)/*check if labelT is null*/
		return NULL;
	for(i=0;i<labelT->size;i++)
		if(strcmp(((labelT->labelAd+i)->label),label) == 0)
			return (labelT->labelAd + i);
	return NULL;
}

void printLbTable(void){
	int i=0;
	if(labelT==NULL)
		printf("label table is empty\n");
	else for(i=0;i<labelT->size;i++)
		printf("label: %s address: %d label type: %d\n",(labelT->labelAd + i)->label,(labelT->labelAd + i)->address,(labelT->labelAd + i)->labelType);
}

int isReg(char *str){
	
	int i=0;
	for(i=0;i<REG_NUM;i++){
		if(strcmp(reg[i],str) == 0)
			return i;
		}
	return -1;
}

int getRegWord(int reg,int offset){
	
	int word=0;
	word|= reg<<offset;/*allocate the register in the offset bit place*/
	word|=A_THREE;/*turn on the A bit from ARE*/
	return word;
}

int getLabelWord(labelAd* label){
	
	int word=0;
	if(label->labelType ==DATA_LABEL)
		word|=((label->address+DC)<<3);/*label address in left 12 bits*/
	else	word|=((label->address)<<3);/*label address in left 12 bits*/
	if(label->labelType==EX_LABEL || label->labelType==ENTRY_LABEL)
		word|=A_ONE;/*turn on the R bit from ARE*/
	else	word|=A_TWO;/*turn on the E bit from ARE*/
	return word;
}

int getDirectWord(char *num){
	
	int word=0;
	word|=(atoi(num+1)<<3);/*move the value to the last 12 bits*/
	word|=A_THREE;/*turn on the A bit from ARE*/
	return word; 
}

void printBinary(unsigned x){

    if (x != 1 && x != 0) 
        printBinary(x>>1); 
  
    printf("%d", x & 1); 
} 

void freeMemory(){
	/*free RAM*/
	if(memory!=NULL){
		free(memory->word);
		free(memory);
	}
	if(labelT!=NULL){
		free(labelT->labelAd);
		free(labelT);
	}
}


