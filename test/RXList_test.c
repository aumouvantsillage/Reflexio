
#include "Reflexio.h"

#include <assert.h>

void main(void) {
    Reflexio_setup();
    
    RXObject_t* list = RXObject_respondTo((RXObject_t*)RXList_o, RXSymbol_symbolForCString("new"), 0);
    RXObject_retain(list);

    RXNativeMethod_push((RXObject_t*)RXInteger_new(125));
    RXObject_respondTo(list, RXSymbol_symbolForCString("append"), 1);
    RXNativeMethod_push((RXObject_t*)RXInteger_new(250));
    RXObject_respondTo(list, RXSymbol_symbolForCString("append"), 1);
    RXNativeMethod_push((RXObject_t*)RXInteger_new(375));
    RXObject_respondTo(list, RXSymbol_symbolForCString("append"), 1);
    RXNativeMethod_push((RXObject_t*)RXInteger_new(500));
    RXObject_respondTo(list, RXSymbol_symbolForCString("append"), 1);

    RXObject_respondTo(list, RXSymbol_symbolForCString("print"), 0);

    RXObject_release(list);
    
    Reflexio_clean();
}
