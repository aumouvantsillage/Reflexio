
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup();
    
    RXInteger_t* n = RXInteger_new(125);
    RXObject_retain((RXObject_t*)n);

    RXObject_respondTo((RXObject_t*)n, RXSymbol_symbolForCString("print"), 0);

    RXObject_release(((RXObject_t*)n));
    
    Reflexio_clean();
}
