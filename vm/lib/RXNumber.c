
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
    RXNumber_o = RXObject_spawn(RXObject_o);    
    RXNativeMethod_attach(RXNumber, asString);
    RXNativeMethod_attach(RXNumber, print);

    RXSymbol_Number_o = RXSymbol_symbolForCString("Number");
    RXObject_setSlot(RXLobby_o, RXSymbol_Number_o, RXNumber_o);
}

void RXNumber_clean(void) {
    RXObject_deleteSlot(RXNumber_o, RXSymbol_delegate_o);
}
