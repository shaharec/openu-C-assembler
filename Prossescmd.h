#include "DataStruct.h"
#include "Syntax.h"
#include "FirstPassUtils.h"
#include "SecondPassUtils.h"
boolean prossesAsm(const char* fileName);	/*prosses the assembler with file name*/
boolean firstPass(FILE *fp);			/*preform first pass*/
boolean secondPass(FILE *fp);			/*preform second pass*/

