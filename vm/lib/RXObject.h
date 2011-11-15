
#ifndef __RX_LIB_OBJECT_H__
#define __RX_LIB_OBJECT_H__

#include <core/RXCore.h>

extern RXObject_t* RXSymbol_value_o;
extern RXObject_t* RXSymbol_print_o;
extern RXObject_t* RXSymbol_asString_o;

void RXObject_libSetup(void);

#define RXObject_valueOfArgumentAt(index) RXObject_respondTo(RXNativeMethod_argumentAt(index), RXSymbol_value_o, 0)

#endif
