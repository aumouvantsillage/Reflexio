
#include "RXCore.h"

RXObject_t* RXNil_o;
RXObject_t* RXObject_o;

RXObject_t* RXSymbol_o;
RXObject_t* RXSymbol_activate_o;
RXObject_t* RXSymbol_delegate_o;
RXObject_t* RXSymbol_lookup_o;

RXObject_t* RXNativeMethod_o;

static RXNativeMethod_define(RXNativeMethod, default) {
    return self;
}

void RXCore_setup(void) {
    RXSymbol_setup();
    RXNativeMethod_setup();

    RXObject_o = RXObject_new();
    
    // Bootstrap root symbol and delegate symbol
    RXSymbol_o = RXSymbol_new("Symbol");
    RXSymbol_delegate_o = RXSymbol_new("delegate");    
    RXObject_setSlot(RXSymbol_o, RXSymbol_delegate_o, RXObject_o);
    RXObject_setSlot(RXSymbol_delegate_o, RXSymbol_delegate_o, RXSymbol_o);

    // Set delegate of Object
    RXObject_setSlot(RXObject_o, RXSymbol_delegate_o, RXObject_o);

    // Delegate is set automatically for these symbols
    RXNil_o = RXObject_spawn(RXObject_o);
    RXSymbol_activate_o = RXSymbol_symbolForCString("activate");
    RXSymbol_lookup_o = RXSymbol_symbolForCString("lookup");

    RXNativeMethod_o = RXNativeMethod_new(RXNativeMethod_functionName(RXNativeMethod, default));
    RXObject_setSlot(RXNativeMethod_o, RXSymbol_delegate_o, RXObject_o);
}

void RXCore_clean(void) {
    RXNativeMethod_clean();
    RXSymbol_clean();
}
