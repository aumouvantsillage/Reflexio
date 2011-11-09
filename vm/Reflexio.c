
#include "Reflexio.h"

void Reflexio_setup(void) {
    RXObject_setup();
    RXSymbol_setup();
    RXNativeMethod_setup();
}

void Reflexio_clean(void) {
    RXObject_clean();
    RXSymbol_clean();
    RXNativeMethod_clean();
}
