
#include "Reflexio.h"

void Reflexio_setup(void) {
    RXCore_setup();
    
    RXObject_attachMethods();
    RXSymbol_attachMethods();
}

void Reflexio_clean(void) {
    RXCore_clean();
}
