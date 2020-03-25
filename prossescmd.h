#include "DataStruct.h"
#include "syntax.h"
boolean prossesAsm(const char* fileName);
boolean firstPass(FILE *fp);
boolean secondPass(FILE *fp);
boolean getword(lineWords *words);
boolean lineFirstPass(char* line, int findex);
boolean handleIC(lnType lineType,char *line,lineWords *words);
void freeWords(lineWords *words);
boolean lineSecondPass(char* line, int findex);
boolean handleRAMWords(lnType lineType,char *line,lineWords *words);
boolean getNextWordInLine(char* line,lineWords *words);
boolean DataRAMWords(char *line,lineWords *words);
boolean	InstRAMWords(char *line,lineWords *words);
