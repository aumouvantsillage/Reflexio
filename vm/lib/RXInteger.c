
#include "RXLib.h"

// Private -------------------------------------------------------------

RXObject_defineType(RXInteger_t, int);

#define RXInteger_payload(self) ((RXInteger_t*)self)->payload

static RXObject_t* RXInteger_new(int value) {
    RXObject_t* self = RXObject_allocateType(RXInteger_t);
    RXObject_initialize(self);
    RXInteger_payload(self) = value;
    return self;
}

// Methods -------------------------------------------------------------

RXNativeMethod_define(RXInteger, spawn) {
    return RXInteger_spawn(self, *(int*)self);
}

RXNativeMethod_define(RXInteger, asString) {
    char* str;
    asprintf(&str, "%d", *(int*)self);
    RXObject_t* result = RXSymbol_symbolForCString(str);
    free(str);
    return result;
}

RXNativeMethod_define(RXInteger, print) {
    printf("%d", *(int*)self);
    return self;
}

RXNativeMethod_define(RXInteger, asBoolean) {
    return RXInteger_payload(self)
        ? RXBoolean_true_o
        : RXBoolean_false_o;
}

// Public --------------------------------------------------------------

RXObject_t* RXInteger_o;
RXObject_t* RXSymbol_Integer_o;

RXObject_t* RXInteger_spawn(RXObject_t* self, int value) {
    RXObject_t* result = RXInteger_new(value);
    RXObject_setSlot(result, RXSymbol_delegate_o, self);    
    return result;
}

void RXInteger_setup(void) {
    RXSymbol_Integer_o = RXSymbol_symbolForCString("Integer");
    
    RXInteger_o = RXInteger_new(0);
    RXObject_setSlot(RXInteger_o, RXSymbol_delegate_o, RXNumber_o);
    
    RXNativeMethod_attach(RXInteger, spawn);
    RXNativeMethod_attach(RXInteger, asString);
    RXNativeMethod_attach(RXInteger, print);
    RXNativeMethod_attach(RXInteger, asBoolean);

    RXObject_setSlot(RXLobby_o, RXSymbol_Integer_o, RXInteger_o);
}
