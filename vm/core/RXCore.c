
#include "RXCore.h"
#include "RXCache.h"

RXObject_t* RXNil_o;
RXObject_t* RXObject_o;
RXObject_t* RXVTable_o;
RXObject_t* RXSlot_o;

RXObject_t* RXSymbol_o;
RXObject_t* RXSymbol_activate_o;
RXObject_t* RXSymbol_delegate_o;
RXObject_t* RXSymbol_lookup_o;
RXObject_t* RXSymbol_respondTo_o;

RXObject_t* RXNativeMethod_o;
RXObject_t* RXNativeMethod_methodNotFound_o;

RXNativeMethod_define(NativeMethod, default) {
    return self;
}

void RXCore_setup(void) {
    RXCache_setup();
    RXSymbol_setup();
    RXNativeMethod_setup();

    RXVTable_o = RXVTable_new();
    RXObject_vtable(RXVTable_o) = RXVTable_o;

    // TODO create RXNativeMethod_methodNotFound_o

    RXObject_o = RXObject_new();
    RXNil_o = RXObject_new();
    RXSlot_o = RXSlot_new(RXNil_o);
    
    // Bootstrap root symbol and delegate symbol
    RXSymbol_o = RXSymbol_new("Symbol");
    RXSymbol_delegate_o = RXSymbol_new("delegate");
    RXObject_setDelegate(RXSymbol_o, RXObject_o);
    RXObject_setDelegate(RXSymbol_delegate_o, RXSymbol_o);

    // Set delegate of Object, Slot and nil
    RXObject_setDelegate(RXObject_o, RXObject_o);
    RXObject_setDelegate(RXNil_o, RXObject_o);

    // Delegate is set automatically for these symbols
    RXSymbol_activate_o = RXSymbol_symbolForCString("activate");
    RXSymbol_lookup_o = RXSymbol_symbolForCString("lookup");
    RXSymbol_respondTo_o = RXSymbol_symbolForCString("respondTo");

    RXNativeMethod_o = RXNativeMethod_spawn(RXObject_o, RXNativeMethod_functionName(NativeMethod, default));
    RXObject_setDelegate(RXSlot_o, RXNativeMethod_o);
}

void RXCore_clean(void) {
    RXNativeMethod_clean();
    RXSymbol_clean();
    RXCache_clean();
}
