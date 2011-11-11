
#include "core/RXCore.h"

static RXNativeMethod_define(RXSymbol, asString) {
    return self;
}

static RXNativeMethod_define(RXSymbol, print) {
    fputs((char*)self, stdout);
    return self;
}

void RXSymbol_libSetup(void) {
    RXObject_setSlot(RXObject_o, RXSymbol_o, (RXObject_t*)RXSymbol_o);
    
    RXNativeMethod_attach(RXSymbol, asString);
    RXNativeMethod_attach(RXSymbol, print);
}
