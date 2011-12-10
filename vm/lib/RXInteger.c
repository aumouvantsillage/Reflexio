
#include "RXLib.h"

// Private -------------------------------------------------------------

RXObject_defineType(Integer,
    int value;
);

#define RXInteger_value(self) ((RXInteger_t*)self)->value

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
    return RXInteger_value(self)
        ? RXBoolean_true_o
        : RXBoolean_false_o;
}

// Public --------------------------------------------------------------

RXObject_t* RXInteger_o;
RXObject_t* RXSymbol_Integer_o;

RXObject_t* RXInteger_spawn(RXObject_t* self, int value) {
    RXObject_t* result = RXObject_allocateType(RXInteger_t);
    RXObject_initialize(result);
    RXObject_setDelegate(result, self);
    RXInteger_value(result) = value;
    return result;
}

void RXInteger_setup(void) {
    RXSymbol_Integer_o = RXSymbol_symbolForCString("Integer");
    
    RXInteger_o = RXInteger_spawn(RXNumber_o, 0);
    
    RXNativeMethod_attach(RXInteger, spawn);
    RXNativeMethod_attach(RXInteger, asString);
    RXNativeMethod_attach(RXInteger, print);
    RXNativeMethod_attach(RXInteger, asBoolean);

    RXObject_setSlot(RXLobby_o, RXSymbol_Integer_o, RXInteger_o);
}
