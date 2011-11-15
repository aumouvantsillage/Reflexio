
#include "RXLib.h"

// Private -------------------------------------------------------------

RXObject_defineType(RXInteger_t, int);

static RXObject_t* RXInteger_new(int value) {
    RXObject_t* self = RXCore_allocateObjectOfType(RXInteger_t);
    RXObject_initialize(self);
    ((RXInteger_t*)self)->payload = value;
    return self;
}

// Methods -------------------------------------------------------------

static RXNativeMethod_define(RXInteger, spawn) {
    return RXInteger_spawn(self, *(int*)self);
}

static RXNativeMethod_define(RXInteger, asString) {
    char* str;
    asprintf(&str, "%d", *(int*)self);
    RXObject_t* result = RXSymbol_symbolForCString(str);
    free(str);
    return result;
}

static RXNativeMethod_define(RXInteger, print) {
    printf("%d", *(int*)self);
    return self;
}

// Public --------------------------------------------------------------

RXObject_t* RXInteger_o;

RXObject_t* RXInteger_spawn(RXObject_t* self, int value) {
    RXObject_t* result = RXInteger_new(value);
    RXObject_setSlot(result, RXSymbol_delegate_o, self);    
    return result;
}

void RXInteger_setup(void) {
    RXInteger_o = RXInteger_new(0);
    RXObject_setSlot(RXInteger_o, RXSymbol_delegate_o, RXNumber_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("Integer"), RXInteger_o);
    
    RXNativeMethod_attach(RXInteger, spawn);
    RXNativeMethod_attach(RXInteger, asString);
    RXNativeMethod_attach(RXInteger, print);
}
