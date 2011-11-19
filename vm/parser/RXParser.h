#ifndef __RX_PARSER_H__
#define __RX_PARSER_H__

#include <core/RXCore.h>

RXObject_t* RXParser_expressionFromCString(char* string);
RXObject_t* RXParser_expressionFromCFile(FILE* file);

#endif

