
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup();
    
    RXObject_t* RXSymbol_spawn_o = RXSymbol_symbolForCString("spawn");
    RXObject_t* RXSymbol_print_o = RXSymbol_symbolForCString("print");
    
    RXObject_t* a = RXObject_respondTo(RXObject_o, RXSymbol_spawn_o, RXNil_o, 0);
    assert(a != RXNil_o);
    
    RXObject_t* b = RXObject_respondTo(RXObject_o, RXSymbol_spawn_o, RXNil_o, 0);
    RXObject_t* c = RXObject_respondTo(RXObject_o, RXSymbol_spawn_o, RXNil_o, 0);
    RXObject_t* sb = RXSymbol_symbolForCString("basile");
    RXObject_t* sc = RXSymbol_symbolForCString("carla");
    RXObject_setSlot(a, sb, b);
    RXObject_setSlot(a, sc, c);
    
    RXObject_t* rb = RXObject_valueOfSlot(a, sb);
    assert(b == rb);
    
    RXObject_t* rc = RXObject_valueOfSlot(a, sc);
    assert(c == rc);
    
    RXObject_respondTo(a, RXSymbol_print_o, RXNil_o, 0);
    RXObject_respondTo(sb, RXSymbol_print_o, RXNil_o, 0);
    
    RXNativeMethod_push(RXNil_o);
    RXNativeMethod_push(sb);
    RXObject_respondTo(RXObject_valueOfSlot(RXObject_o, RXSymbol_print_o), RXSymbol_activate_o, RXNil_o, 2);
    
    Reflexio_clean();
}
