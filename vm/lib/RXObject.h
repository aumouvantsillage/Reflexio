
#ifndef __RX_LIB_OBJECT_H__
#define __RX_LIB_OBJECT_H__

void RXObject_libSetup(void);

extern RXSymbol_t* RXSymbol_value_o;
extern RXSymbol_t* RXSymbol_print_o;
static RXSymbol_t* RXSymbol_asString_o;

#define RXObject_valueOfArgumentAt(index) RXObject_respondTo(RXNativeMethod_argumentAt(index), RXSymbol_value_o, 0)

#endif
