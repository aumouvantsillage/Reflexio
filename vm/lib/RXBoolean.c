
#include "RXBoolean.h"

RXObject_t* RXBoolean_o;
RXObject_t* RXBoolean_true_o;
RXObject_t* RXBoolean_false_o;

void RXBoolean_setup(void) {
    RXBoolean_o = RXObject_spawn(RXObject_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("Boolean"), RXBoolean_o);

    RXBoolean_true_o = RXObject_spawn(RXBoolean_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("true"), RXBoolean_true_o);

    RXBoolean_false_o = RXObject_spawn(RXBoolean_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("false"), RXBoolean_false_o);
}
