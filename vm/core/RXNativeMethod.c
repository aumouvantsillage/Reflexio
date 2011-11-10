
#include "RXNativeMethod.h"
#include "RXSymbol.h"

// Public --------------------------------------------------------------

RXObject_t** RXNativeMethod_stackBottom;
RXObject_t** RXNativeMethod_stackTop;

RXNativeMethod_t* RXNativeMethod_new(RXNativeMethodBody_t body) {
    RXNativeMethod_t* self = (RXNativeMethod_t*)RXCore_allocateObjectOfType(RXNativeMethod_t);
    RXObject_initialize((RXObject_t*)self);
    RXObject_setIsNativeMethod((RXObject_t*)self);
    self->payload = body;
    return self;
}

void RXNativeMethod_setup(void) {
    RXNativeMethod_stackBottom = malloc(RX_STACK_SIZE * sizeof(RXObject_t*));
    assert(RXNativeMethod_stackBottom != NULL);
    RXNativeMethod_stackTop = RXNativeMethod_stackBottom;
}

void RXNativeMethod_clean(void) {
    free(RXNativeMethod_stackBottom);
}
