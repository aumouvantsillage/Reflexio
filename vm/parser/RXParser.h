#ifndef __RX_PARSER_H__
#define __RX_PARSER_H__

#include <core/RXCore.h>

RXObject_t* RXParser_expressionFromString(RXObject_t* string);
RXObject_t* RXParser_expressionFromFile(RXObject_t* file);

#endif

