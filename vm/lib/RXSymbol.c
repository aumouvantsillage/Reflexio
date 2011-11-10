
#include "core/RXCore.h"

static RXNativeMethod_define(RXSymbol, asString) {
    return self;
}

static RXNativeMethod_define(RXSymbol, print) {
    puts((char*)self);
    return self;
}

void RXSymbol_attachMethods() {
    RXNativeMethod_attach(RXSymbol, asString);
    RXNativeMethod_attach(RXSymbol, print);
}
