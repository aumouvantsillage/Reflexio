
#include "RXNumber.h"

RXObject_t* RXNumber_o;

void RXNumber_setup(void) {
    RXNumber_o = RXObject_new();
    RXObject_setSlot(RXNumber_o, RXSymbol_delegate_o, RXObject_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("Number"), RXNumber_o);
}

void RXNumber_clean(void) {
    RXObject_deleteSlot((RXObject_t*)RXNumber_o, RXSymbol_delegate_o);
}
