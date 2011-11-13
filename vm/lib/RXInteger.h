
#ifndef __RX_LIB_INTEGER_H__
#define __RX_LIB_INTEGER_H__

#include <core/RXCore.h>

extern RXObject_t* RXInteger_o;

RXObject_t* RXInteger_spawn(RXObject_t* self, int value);

void RXInteger_setup(void);

#endif
