
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup(0, NULL);

    RXObject_t* basile = RXSymbol_symbolForCString("basile");
    RXObject_t* carla = RXSymbol_symbolForCString("carla");
    
    RXObject_t* a = RXObject_respondTo(RXObject_o, RXSymbol_spawn_o, RXNil_o, 0);
    assert(a != RXNil_o);
    assert(a != RXObject_o);
    
    RXObject_t* b = RXObject_respondTo(RXObject_o, RXSymbol_spawn_o, RXNil_o, 0);
    assert(b != RXNil_o);
    assert(b != RXObject_o);
    
    RXObject_t* c = RXObject_respondTo(RXObject_o, RXSymbol_spawn_o, RXNil_o, 0);
    assert(c != RXNil_o);
    assert(c != RXObject_o);
    
    // a setSlot("basile", b)
    RXNativeMethod_push(b);
    RXNativeMethod_push(basile);
    RXObject_respondTo(a, RXSymbol_setSlot_o, RXNil_o, 2);
    assert(b == RXObject_valueOfSlot(a, basile));
    
    // a setSlot("carla", c)
    RXNativeMethod_push(c);
    RXNativeMethod_push(carla);
    RXObject_respondTo(a, RXSymbol_setSlot_o, RXNil_o, 2);
    assert(c == RXObject_valueOfSlot(a, carla));
    
    // b == a valueOfSlot("basile")
    RXNativeMethod_push(basile);
    RXObject_t* rb = RXObject_respondTo(a, RXSymbol_valueOfSlot_o, RXNil_o, 1);
    assert(b == rb);
    
    // c == a valueOfSlot("carla")
    RXNativeMethod_push(carla);
    RXObject_t* rc = RXObject_respondTo(a, RXSymbol_valueOfSlot_o, RXNil_o, 1);
    assert(c == rc);
    
    // nil println
    // -> nil
    RXObject_respondTo(RXNil_o, RXSymbol_println_o, RXNil_o, 0);
    
    // "basile" println
    // -> basile
    RXObject_respondTo(basile, RXSymbol_println_o, RXNil_o, 0);
    
    // a println
    // -> Object_0x...
    RXObject_respondTo(a, RXSymbol_println_o, RXNil_o, 0);
    
    // Object valueOfSlot("asString") activate("basile", nil) println
    // -> Object_0x...
    RXNativeMethod_push(RXSymbol_asString_o);
    RXObject_t* asString = RXObject_respondTo(RXObject_o, RXSymbol_valueOfSlot_o, RXNil_o, 1);
    
    RXNativeMethod_push(RXNil_o);
    RXNativeMethod_push(basile);
    RXObject_t* str = RXObject_respondTo(asString, RXSymbol_activate_o, RXNil_o, 2);
    
    RXObject_respondTo(str, RXSymbol_println_o, RXNil_o, 0);
    
    // Symbol valueOfSlot("asString") activate("basile", nil) println
    // -> basile
    RXNativeMethod_push(RXSymbol_asString_o);
    asString = RXObject_respondTo(RXSymbol_o, RXSymbol_valueOfSlot_o, RXNil_o, 1);
    
    RXNativeMethod_push(RXNil_o);
    RXNativeMethod_push(basile);
    str = RXObject_respondTo(asString, RXSymbol_activate_o, RXNil_o, 2);
    
    RXObject_respondTo(str, RXSymbol_println_o, RXNil_o, 0);
    
    Reflexio_clean();
}
