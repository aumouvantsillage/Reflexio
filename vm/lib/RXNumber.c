
#include "RXLib.h"

RXObject_t* RXSymbol_Number_o;

RXNativeMethod_define(RXNumber, asString) {
    return RXSymbol_Number_o;
}

RXNativeMethod_define(RXNumber, print) {
    fputs("Number", stdout);
    return self;
}

RXObject_t* RXNumber_o;

void RXNumber_setup(void) {
    RXSymbol_Number_o = RXSymbol_symbolForCString("Number");
    
    RXNumber_o = RXObject_new();
    RXObject_setSlot(RXNumber_o, RXSymbol_delegate_o, RXObject_o);
    RXObject_setSlot(RXObject_o, RXSymbol_Number_o, RXNumber_o);
    
    RXNativeMethod_attach(RXNumber, asString);
    RXNativeMethod_attach(RXNumber, print);
}

void RXNumber_clean(void) {
    RXObject_deleteSlot(RXNumber_o, RXSymbol_delegate_o);
}
