
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup();
    
    RXSymbol_t* a = RXSymbol_symbolForCString("albert");
    assert(a != NULL);
    
    RXSymbol_t* b = RXSymbol_symbolForCString("beric");
    RXSymbol_t* c = RXSymbol_symbolForCString("rico");
    RXSymbol_t* d = RXSymbol_symbolForCString("albert");
    assert(a == d);
    
    RXSymbol_dumpAll();
    
    Reflexio_clean();
}
