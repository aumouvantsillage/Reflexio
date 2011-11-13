
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup();
    
    RXObject_t* n = RXInteger_new(125);
    RXObject_retain(n);

    RXObject_respondTo(n, RXSymbol_symbolForCString("print"), 0);

    RXObject_release(n);
    
    Reflexio_clean();
}
