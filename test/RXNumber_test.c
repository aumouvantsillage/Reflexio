
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup();
    
    RXObject_t* n = RXInteger_spawn(RXInteger_o, 125);

    RXObject_respondTo(n, RXSymbol_symbolForCString("print"), 0);

    Reflexio_clean();
}
