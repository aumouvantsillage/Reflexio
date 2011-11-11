
#include "Reflexio.h"

void Reflexio_setup(void) {
    RXCore_setup();
    
    RXObject_libSetup();
    RXSymbol_libSetup();
    
    RXList_setup();
}

void Reflexio_clean(void) {
    RXList_clean();
    
    RXCore_clean();
}
