#include "DataStruct.h"
boolean prossesAsm(const char* fileName);
boolean firstPass(FILE *fp);
boolean secondPass(FILE *fp);
boolean getword(char* line,char **word, int num);
boolean lineFirstPass(char* line, int findex);
void handleIC(lnType lineType,char *line,lineWords *words);
void freeWords(lineWords *words);
boolean lineSecondPass(char* line, int findex);
void handleRAMWords(lnType lineType,char *line,lineWords *words);
