#ifndef LINE_UTILS_H
#define LINE_UTILS_H

#include "DataStruct.h"
/*prototypes*/
boolean getword(lineWords *words);				/*fill the next word in the words variable*/
boolean getNextWordInLine(char* line,lineWords *words);		/*get the next word in the line*/
void freeWords(lineWords *words);				/*free allocation of words*/
int isReg(char *str);						/*check if str is register*/

#endif /* defined(LINE_UTILS_H) */
