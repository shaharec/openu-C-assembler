#include "DataStruct.h"

int IC=0;/*instruction counter for memory*/
int DC=0;/*data counter for memory*/

labelTable* labelT = NULL;
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

boolean Islabel(const char *str){			
    
    int len = strlen(str),i=0;
    if(str && *str && (str[len - 1] == ':')){  
     	if (len - 1 > 30) {/*if label not within 30 chars, if yes it will print error but won't compile but will keep parsing*/
       		printf("more then 30 character for label\n");	 
      	}    
        for(i=0;i<len-2;i++) /*in a label it should be only digits or letters, else it will print error but will keep parsing*/
            if(!isalpha(*(str + len)) && (*(str + len) > '9' || *(str + len) < '0' )){
                printf("invalid label\n");
            }
         return true;   
    }
    return false;
}


boolean addLb(char* label, lbType labelType){
   
	if(labelExsist(label)) {
		prinf("label exist in table");
      		return false;
    }
	if (labelT!=NULL) {
		labelT = calloc(1, sizeof(labelTable));
	        labelT->size = 0;
	        labelT->labelAd = calloc(1, sizeof(labelAd));
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

boolean isCmd(char *str){
	int i=0;
	for(i=0;i<OP_SIZE;i++)
		if(strcmp(opcode[i],str) == 0)
			return true;
	return false;
		
}
