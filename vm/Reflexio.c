
#include "Reflexio.h"

RXObject_t* RXNil_o;
RXObject_t* RXObject_o;

RXSymbol_t* RXSymbol_o;
RXSymbol_t* RXSymbol_activate_o;
RXSymbol_t* RXSymbol_delegate_o;
RXSymbol_t* RXSymbol_lookup_o;
RXSymbol_t* RXSymbol_new_o;

RXNativeMethod_t* RXNativeMethod_o;

static RXNativeMethod_define(RXObject, new) {
    RXObject_t* result = RXObject_new();
    RXObject_setSlot(result, RXSymbol_delegate_o, self);
    return result;
}

static RXNativeMethod_define(RXNativeMethod, default) {
    return self;
}

void Reflexio_setup(void) {
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
    RXSymbol_new_o = RXSymbol_symbolForCString("new");

    RXNativeMethod_o = RXNativeMethod_new(RXNativeMethod_functionName(RXNativeMethod, default));
    RXObject_setSlot((RXObject_t*)RXNativeMethod_o, RXSymbol_delegate_o, RXObject_o);

    RXNativeMethod_attach(RXObject, new);
    
    // These symbols are not assigned currently
    RXObject_retain(RXNil_o);
    RXObject_retain((RXObject_t*)RXSymbol_lookup_o);
    RXObject_retain((RXObject_t*)RXSymbol_activate_o);
}

void Reflexio_clean(void) {
    RXObject_deleteSlot(RXObject_o, RXSymbol_delegate_o);
    RXObject_deleteSlot(RXNil_o, RXSymbol_delegate_o);
    RXObject_deleteSlot((RXObject_t*)RXSymbol_o, RXSymbol_delegate_o);
    RXObject_deleteSlot((RXObject_t*)RXSymbol_delegate_o, RXSymbol_delegate_o);
    RXObject_deleteSlot((RXObject_t*)RXNativeMethod_o, RXSymbol_delegate_o);

    RXObject_release(RXNil_o);
    RXObject_release((RXObject_t*)RXSymbol_lookup_o);
    RXObject_release((RXObject_t*)RXSymbol_activate_o);

    RXSymbol_clean();
    RXNativeMethod_clean();
}
