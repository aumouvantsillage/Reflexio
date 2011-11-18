
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
RXObject_t* RXSymbol_equal_o;
RXObject_t* RXSymbol_notEqual_o;
RXObject_t* RXSymbol_lessThan_o;
RXObject_t* RXSymbol_greaterThan_o;
RXObject_t* RXSymbol_lessOrEqual_o;
RXObject_t* RXSymbol_greaterOrEqual_o;
RXObject_t* RXSymbol_add_o;
RXObject_t* RXSymbol_subtract_o;
RXObject_t* RXSymbol_multiply_o;
RXObject_t* RXSymbol_divide_o;
RXObject_t* RXSymbol_unaryPlus_o;
RXObject_t* RXSymbol_negate_o;

void RXNumber_setup(void) {
    RXSymbol_equal_o = RXSymbol_symbolForCString("equal");
    RXSymbol_notEqual_o = RXSymbol_symbolForCString("notEqual");
    RXSymbol_lessThan_o = RXSymbol_symbolForCString("lessThan");
    RXSymbol_greaterThan_o = RXSymbol_symbolForCString("greaterThan");
    RXSymbol_lessOrEqual_o = RXSymbol_symbolForCString("lessOrEqual");
    RXSymbol_greaterOrEqual_o = RXSymbol_symbolForCString("greaterOrEqual");
    RXSymbol_add_o = RXSymbol_symbolForCString("add");
    RXSymbol_subtract_o = RXSymbol_symbolForCString("subtract");
    RXSymbol_multiply_o = RXSymbol_symbolForCString("multiply");
    RXSymbol_divide_o = RXSymbol_symbolForCString("divide");
    RXSymbol_unaryPlus_o = RXSymbol_symbolForCString("unaryPlus");
    RXSymbol_negate_o = RXSymbol_symbolForCString("negate");

    RXNumber_o = RXObject_spawn(RXObject_o);    
    RXNativeMethod_attach(RXNumber, asString);
    RXNativeMethod_attach(RXNumber, print);

    RXSymbol_Number_o = RXSymbol_symbolForCString("Number");
    RXObject_setSlot(RXLobby_o, RXSymbol_Number_o, RXNumber_o);
}

void RXNumber_clean(void) {
    RXObject_deleteSlot(RXNumber_o, RXSymbol_delegate_o);
}
