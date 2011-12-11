
#include "RXCore.h"
#include "RXCache.h"

RXObject_t* RXNil_o;
RXObject_t* RXProtoObject_o;
RXObject_t* RXObject_o;
RXObject_t* RXLobby_o;
RXObject_t* RXSymbol_Lobby_o;
RXObject_t* RXSymbol_Object_o;
RXObject_t* RXSymbol_ProtoObject_o;
RXObject_t* RXSymbol_nil_o;
RXObject_t* RXSymbol_NativeMethod_o;

RXObject_t* RXSymbol_o;
RXObject_t* RXSymbol_activate_o;
RXObject_t* RXSymbol_delegate_o;
RXObject_t* RXSymbol_lookup_o;
RXObject_t* RXSymbol_respondTo_o;

RXObject_t* RXNativeMethod_o;

RXNativeMethod_define(RXNativeMethod, default) {
    return self;
}

void RXCore_setup(void) {
    RXCache_setup();
    RXSymbol_setup();
    RXNativeMethod_setup();

    // Creating core objects
    RXNil_o = RXObject_new();
    RXProtoObject_o = RXObject_new();
    RXLobby_o = RXObject_new();
    RXObject_o = RXObject_new();
    RXSymbol_o = RXSymbol_new("Symbol");
    RXNativeMethod_o = RXNativeMethod_new(RXNativeMethod_functionName(RXNativeMethod, default));
    
    // Setting delegates
    RXSymbol_delegate_o = RXSymbol_new("delegate");

    RXObject_setSlot(RXNil_o, RXSymbol_delegate_o, RXNil_o);
    RXObject_setSlot(RXProtoObject_o, RXSymbol_delegate_o, RXNil_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_delegate_o, RXProtoObject_o);
    RXObject_setSlot(RXObject_o, RXSymbol_delegate_o, RXLobby_o);   
    RXObject_setSlot(RXSymbol_o, RXSymbol_delegate_o, RXObject_o);
    RXObject_setSlot(RXSymbol_delegate_o, RXSymbol_delegate_o, RXSymbol_o);
    RXObject_setSlot(RXNativeMethod_o, RXSymbol_delegate_o, RXObject_o);

    // Registering core objects in the lobby
    RXSymbol_nil_o = RXSymbol_symbolForCString("nil");
    RXSymbol_ProtoObject_o = RXSymbol_symbolForCString("ProtoObject");
    RXSymbol_Lobby_o = RXSymbol_symbolForCString("Lobby");
    RXSymbol_Object_o = RXSymbol_symbolForCString("Object");

    RXObject_setSlot(RXLobby_o, RXSymbol_nil_o, RXNil_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_ProtoObject_o, RXProtoObject_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_Lobby_o, RXLobby_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_Object_o, RXObject_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_o, RXSymbol_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_NativeMethod_o, RXNativeMethod_o);
    
    // Creating symbols for core methods
    RXSymbol_activate_o = RXSymbol_symbolForCString("activate");
    RXSymbol_lookup_o = RXSymbol_symbolForCString("lookup");
    RXSymbol_respondTo_o = RXSymbol_symbolForCString("respondTo");
}

void RXCore_clean(void) {
    RXNativeMethod_clean();
    RXSymbol_clean();
    RXCache_clean();
}
