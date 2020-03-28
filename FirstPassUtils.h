#include "DataStruct.h"
#include "LineUtils.h"
/*prototypes*/
boolean lineFirstPass(char* line, int findex);			/*preform the first line pass logic on the line with findex index*/
boolean handleIC(lnType lineType,char *line,lineWords *words);	/*update instruction counter by the line type and words*/
