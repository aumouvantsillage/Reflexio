
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup();
    
    RXSymbol_t* RXSymbol_new_o = RXSymbol_symbolForCString("new");
    RXSymbol_t* RXSymbol_print_o = RXSymbol_symbolForCString("print");
    
    RXObject_t* a = RXObject_respondTo(RXObject_o, RXSymbol_new_o, 0);
    assert(a != RXNil_o);
    RXObject_retain(a);
    
    RXObject_t* b = RXObject_respondTo(RXObject_o, RXSymbol_new_o, 0);
    RXObject_t* c = RXObject_respondTo(RXObject_o, RXSymbol_new_o, 0);
    RXSymbol_t* sb = RXSymbol_symbolForCString("basile");
    RXSymbol_t* sc = RXSymbol_symbolForCString("carla");
    RXObject_setSlot(a, sb, b);
    RXObject_setSlot(a, sc, c);
    
    RXObject_t* rb = RXObject_valueOfSlot(a, sb);
    assert(b == rb);
    
    RXObject_t* rc = RXObject_valueOfSlot(a, sc);
    assert(c == rc);
    
    RXObject_respondTo(a, RXSymbol_print_o, 0);
    RXObject_respondTo((RXObject_t*)sb, RXSymbol_print_o, 0);
    
    RXNativeMethod_push((RXObject_t*)sb);
    RXObject_respondTo(RXObject_valueOfSlot(RXObject_o, RXSymbol_print_o), RXSymbol_activate_o, 1);
    
    RXObject_release(a);
    
    Reflexio_clean();
}
