
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup();
    
    RXObject_t* a = RXObject_respondTo(RXObject_o, RXSymbol_new_o);
    RXObject_retain(a);
    
    RXObject_t* b = RXObject_respondTo(RXObject_o, RXSymbol_new_o);
    RXObject_t* c = RXObject_respondTo(RXObject_o, RXSymbol_new_o);
    RXSymbol_t* sb = RXSymbol_symbolForCString("basile");
    RXSymbol_t* sc = RXSymbol_symbolForCString("carla");
    RXObject_setSlot(a, sb, b);
    RXObject_setSlot(a, sc, c);
    
    RXObject_t* rb = RXObject_valueOfSlot(a, sb);
    assert(b == rb);
    
    RXObject_t* rc = RXObject_valueOfSlot(a, sc);
    assert(c == rc);
    
    RXObject_release(a);
    
    Reflexio_clean();
}
