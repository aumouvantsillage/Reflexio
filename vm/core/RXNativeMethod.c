
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

RXObject_t* RXNativeMethod_new(RXNativeMethodBody_t body) {
    RXObject_t* self = RXObject_allocateType(RXNativeMethod_t);
    RXObject_initialize(self);
    RXObject_setIsNativeMethod(self);
    ((RXNativeMethod_t*)self)->body = body;
    return self;
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
