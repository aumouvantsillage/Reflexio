
#ifndef __RX_LIB_INTEGER_H__
#define __RX_LIB_INTEGER_H__

#include <core/RXCore.h>

RXObject_defineType(RXInteger_t, int);

RXInteger_t* RXInteger_new(int value);

void RXInteger_setup(void);
void RXInteger_clean(void);

#endif
