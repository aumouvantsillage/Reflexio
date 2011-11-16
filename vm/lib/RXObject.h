
#ifndef __RX_LIB_OBJECT_H__
#define __RX_LIB_OBJECT_H__

#include <core/RXCore.h>

extern RXObject_t* RXSymbol_spawn_o;
extern RXObject_t* RXSymbol_setSlot_o;
extern RXObject_t* RXSymbol_valueOfSlot_o;
extern RXObject_t* RXSymbol_deleteSlot_o;
extern RXObject_t* RXSymbol_valueInContext_o;
extern RXObject_t* RXSymbol_print_o;
extern RXObject_t* RXSymbol_println_o;
extern RXObject_t* RXSymbol_asString_o;
extern RXObject_t* RXSymbol_nil_o;

void RXObject_libSetup(void);

inline static RXObject_t* RXObject_valueOfArgumentAt(int index, RXObject_t* context) {
    RXObject_t* receiver = RXNativeMethod_argumentAt(index);
    RXNativeMethod_push(context);
    return RXObject_respondTo(receiver, RXSymbol_valueInContext_o, RXNil_o, 1);
}

#endif
