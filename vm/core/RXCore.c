
#include "RXCore.h"

RXObject_t* RXNil_o;
RXObject_t* RXObject_o;

RXSymbol_t* RXSymbol_o;
RXSymbol_t* RXSymbol_activate_o;
RXSymbol_t* RXSymbol_delegate_o;
RXSymbol_t* RXSymbol_lookup_o;

RXNativeMethod_t* RXNativeMethod_o;

static RXNativeMethod_define(RXNativeMethod, default) {
    return self;
}

void RXCore_setup(void) {
    RXSymbol_setup();
    RXNativeMethod_setup();

    RXObject_o = RXObject_new();
    RXNil_o = RXObject_new();
    
    // Bootstrap root symbol and delegate symbol
    RXSymbol_o = RXSymbol_new("Symbol");
    RXSymbol_delegate_o = RXSymbol_new("delegate");    
    RXObject_setSlot((RXObject_t*)RXSymbol_o, RXSymbol_delegate_o, RXObject_o);
    RXObject_setSlot((RXObject_t*)RXSymbol_delegate_o, RXSymbol_delegate_o, (RXObject_t*)RXSymbol_o);

    // Set delegates of Object and nil
    RXObject_setSlot(RXObject_o, RXSymbol_delegate_o, RXObject_o);
    RXObject_setSlot(RXNil_o, RXSymbol_delegate_o, RXObject_o);    

    // Delegate is set automatically for these symbols
    RXSymbol_activate_o = RXSymbol_symbolForCString("activate");
    RXSymbol_lookup_o = RXSymbol_symbolForCString("lookup");

    RXNativeMethod_o = RXNativeMethod_new(RXNativeMethod_functionName(RXNativeMethod, default));
    RXObject_setSlot((RXObject_t*)RXNativeMethod_o, RXSymbol_delegate_o, RXObject_o);

    
    // These objects are not assigned to slots, so they will be released manually
    RXObject_retain(RXNil_o);
}

void RXCore_clean(void) {
    RXObject_deleteSlot(RXObject_o, RXSymbol_delegate_o);
    RXObject_deleteSlot(RXNil_o, RXSymbol_delegate_o);
    RXObject_deleteSlot((RXObject_t*)RXSymbol_o, RXSymbol_delegate_o);
    RXObject_deleteSlot((RXObject_t*)RXSymbol_delegate_o, RXSymbol_delegate_o);
    RXObject_deleteSlot((RXObject_t*)RXNativeMethod_o, RXSymbol_delegate_o);

    // These objects are not assigned to slots, so they will be released manually
    RXObject_release(RXNil_o);

    RXSymbol_clean();
    RXNativeMethod_clean();
}
