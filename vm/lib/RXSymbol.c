
#include "core/RXCore.h"

static RXNativeMethod_define(RXSymbol, asString) {
    return self;
}

static RXNativeMethod_define(RXSymbol, print) {
    fputs((char*)self, stdout);
    return self;
}

RXObject_t* RXSymbol_value_o;
RXObject_t* RXSymbol_asString_o;
RXObject_t* RXSymbol_print_o;

void RXSymbol_libSetup(void) {
    RXSymbol_value_o = RXSymbol_symbolForCString("value");
    RXSymbol_asString_o = RXSymbol_symbolForCString("asString");
    RXSymbol_print_o = RXSymbol_symbolForCString("print");
    
    RXObject_setSlot(RXObject_o, RXSymbol_o, RXSymbol_o);
    
    RXNativeMethod_attach(RXSymbol, asString);
    RXNativeMethod_attach(RXSymbol, print);
}
