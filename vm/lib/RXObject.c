
#include "core/RXCore.h"

static RXNativeMethod_define(RXObject, new) {
    RXObject_t* result = RXObject_new();
    RXObject_setSlot(result, RXSymbol_delegate_o, self);
    return result;
}

void RXObject_attachMethods() {
    RXNativeMethod_attach(RXObject, new);
}
