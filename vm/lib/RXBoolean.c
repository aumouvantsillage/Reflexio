
#include "RXBoolean.h"

RXObject_t* RXBoolean_o;
RXObject_t* RXBoolean_true_o;
RXObject_t* RXBoolean_false_o;

void RXBoolean_setup(void) {
    RXBoolean_o = RXObject_new();
    RXObject_setSlot(RXBoolean_o, RXSymbol_delegate_o, RXObject_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("Boolean"), RXBoolean_o);

    RXBoolean_true_o = RXObject_new();
    RXObject_setSlot(RXBoolean_true_o, RXSymbol_delegate_o, RXBoolean_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("true"), RXBoolean_true_o);

    RXBoolean_false_o = RXObject_new();
    RXObject_setSlot(RXBoolean_false_o, RXSymbol_delegate_o, RXBoolean_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("false"), RXBoolean_false_o);
}

void RXBoolean_clean(void) {
    RXObject_deleteSlot(RXBoolean_o, RXSymbol_delegate_o);
    RXObject_deleteSlot(RXBoolean_true_o, RXSymbol_delegate_o);
    RXObject_deleteSlot(RXBoolean_false_o, RXSymbol_delegate_o);
}
