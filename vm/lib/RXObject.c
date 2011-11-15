
#include "RXLib.h"
#include <inttypes.h>

static RXNativeMethod_define(RXObject, spawn) {
    return RXObject_spawn(self);
}

#define RXObject_format ("Object_0x%" PRIxPTR)

static RXNativeMethod_define(RXObject, asString) {
    char* str;
    asprintf(&str, RXObject_format, self);
    RXObject_t* result = RXSymbol_symbolForCString(str);
    free(str);
    return result;
}

static RXNativeMethod_define(RXObject, print) {
    printf(RXObject_format, self);
    return self;
}

static RXNativeMethod_define(RXObject, value) {
    return self;
}

RXObject_t* RXSymbol_value_o;
RXObject_t* RXSymbol_asString_o;
RXObject_t* RXSymbol_print_o;

void RXObject_libSetup(void) {
    RXSymbol_value_o = RXSymbol_symbolForCString("value");
    RXSymbol_asString_o = RXSymbol_symbolForCString("asString");
    RXSymbol_print_o = RXSymbol_symbolForCString("print");
    
    RXNativeMethod_attach(RXObject, spawn);
    RXNativeMethod_attach(RXObject, asString);
    RXNativeMethod_attach(RXObject, print);
    RXNativeMethod_attach(RXObject, value);
}

void RXObject_libClean(void) {
    // Nothing
}
