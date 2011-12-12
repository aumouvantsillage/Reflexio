
#include "RXCore.h"

// Private -------------------------------------------------------------

/*
 * A native method is an object with a reference to a C function.
 */
RXObject_defineType(RXNativeMethod_t,
    RXNativeMethodBody_t body;
);


// Public --------------------------------------------------------------

RXObject_t** RXNativeMethod_stackBottom;
RXObject_t** RXNativeMethod_stackTop;

RXObject_t* RXNativeMethod_spawn(RXObject_t* self, RXNativeMethodBody_t body) {
    RXObject_t* result = RXObject_allocateType(RXNativeMethod_t);
    RXObject_initialize(result);
    RXObject_setIsNativeMethod(result);
    ((RXNativeMethod_t*)result)->body = body;
    RXObject_setDelegate(result, self);
    return result;
}

void RXNativeMethod_setup(void) {
    RXNativeMethod_stackBottom = malloc(RX_STACK_SIZE * sizeof(RXObject_t*));
    assert(RXNativeMethod_stackBottom != NULL);
    RXNativeMethod_stackTop = RXNativeMethod_stackBottom;
}

void RXNativeMethod_clean(void) {
    assert(RXNativeMethod_stackTop == RXNativeMethod_stackBottom);
    free(RXNativeMethod_stackBottom); // TODO remove this function when using GC
}
