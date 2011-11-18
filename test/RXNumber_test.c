
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup(0, NULL);
    
    RXObject_t* n = RXInteger_spawn(RXInteger_o, 125);

    RXObject_respondTo(n, RXSymbol_symbolForCString("print"), RXNil_o, 0);

    Reflexio_clean();
}
