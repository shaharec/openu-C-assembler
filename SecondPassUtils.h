#include "DataStruct.h"
#include "LineUtils.h"
boolean lineSecondPass(char* line, int findex);				/*operate second pass logic in the line in row findex*/
boolean handleRAMWords(lnType lineType,char *line,lineWords *words);	/*update the RAM with the right line type and words*/
boolean DataRAMWords(char *line,lineWords *words);			/*update data RAM line words*/
boolean	InstRAMWords(char *line,lineWords *words);			/*update instruction RAM line words*/
int getDirectWord(char *num);						/*return RAM word of direct value*/
int getLabelWord(labelAd* label);					/*return RAM word of label value*/
int getRegWord(int reg,int offset);					/*return RAM word of register value with the offset*/
