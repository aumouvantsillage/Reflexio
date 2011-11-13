
#include "core/RXCore.h"
#include <inttypes.h>

static RXNativeMethod_define(RXObject, new) {
    RXObject_t* result = RXObject_new();
    RXObject_setSlot(result, RXSymbol_delegate_o, self);
    return result;
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

void RXObject_libSetup(void) {
    RXNativeMethod_attach(RXObject, new);
    RXNativeMethod_attach(RXObject, asString);
    RXNativeMethod_attach(RXObject, print);
    RXNativeMethod_attach(RXObject, value);
}

void RXObject_libClean(void) {
    // Nothing
}
