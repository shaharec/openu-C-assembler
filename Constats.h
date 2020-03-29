/*all constants of the assembler program*/
#define LB_NAME_SIZE 		31	 	/*size of lable name*/
#define LINE_LEN 		82		/*max size of line*/
#define OP_SIZE 		16		/*number of command operation in the assembler*/
#define OP_NAME_SIZE 		5		/*max size of command operatuin name*/
#define RAM_WORD_SIZE 		15		/*size of word in the ram memory in bits*/
#define REG_NUM 		8		/*number of registers in the system*/
#define REG_NAME_LEN 		3		/*max size of register name length*/
#define RAM_MAX_SIZE 		4096		/*size of ram memmory available in the PC*/
#define FOR_SBIT 		6 		/*first operand register strat bit*/
#define SOR_SBIT 		3 		/*second operand register strat bit*/
#define FOAM_SBIT 		7	 	/*first operand adressing method start bit*/
#define SOAM_SBIT 		3	 	/*second operand adressing method start bit*/
#define OCAM_SBIT 		11	 	/*opcode start bit*/
#define EO_ARE 			3		/*end of A,R,E bit*/
#define MAX_WORD_INST 		3		/*define max ram words in an instruction*/
#define MEMORY_START		100		/*define the strat address of memory*/
#define DATA_CMD		".data"		/*data commamd in the assembler*/
#define STR_CMD			".string"	/*string commamd in the assembler*/
#define EXT_CMD			".extern"	/*extern commamd in the assembler*/
#define ENT_CMD			".entry"	/*entry commamd in the assembler*/
#define MAX_COMMAND 		8		/*the maxsimom length of command/data/string*/
#define FIRST 			0		/*number of first operand arguments*/
#define SECOND 			1		/*number of second operand arguments*/
#define ONLY 			2		/*number of only operand arguments*/
#define MAX_DATA_CELLS		7		/*the number 16,383‬ represent 14 bits + sign (-,+)*/
#define MAX_DIRECT_CELLS	6		/*the number 16,383‬ represent 14 bits + sign (-,+)*/
#define MAX_15_BIT		16383		/*maximum integer in 15 bit with sign*/
#define MIN_15_BIT		-16384		/*maximum integer in 15 bit with sign*/
#define MAX_12_BIT		2047		/*maximum integer in 12 bit with sign*/
#define MIN_12_BIT		-2048		/*maximum integer in 12 bit with sign*/
