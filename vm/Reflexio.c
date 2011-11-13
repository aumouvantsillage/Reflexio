
#include "Reflexio.h"

void Reflexio_setup(void) {
    eina_init();
    
    RXCore_setup();
    
    RXObject_libSetup();
    RXSymbol_libSetup();
    
    RXBoolean_setup();
    RXNumber_setup();
    RXInteger_setup();
    RXList_setup();
}

void Reflexio_clean(void) {
    RXCore_clean();

    eina_shutdown();
}
