
#include "RXCore.h"
#include <assert.h>

// Private -------------------------------------------------------------

/*
 * A native method is an object with a reference to a C function.
 */
RXObject_defineType(RXNativeMethod_t,
    RXNativeMethodBody_t body;
);

#define RXNativeMethod_body(self) ((RXNativeMethod_t*)self)->body

/*
 * Override constant RX_STACK_SIZE to change the stack size.
 */
#ifndef RX_STACK_SIZE
#define RX_STACK_SIZE 1024
#endif

// Public --------------------------------------------------------------

RXObject_t** RXNativeMethod_stackBottom;
RXObject_t** RXNativeMethod_stackTop;

void RXNativeMethod_setup(void) {
    RXNativeMethod_stackBottom = malloc(RX_STACK_SIZE * sizeof(RXObject_t*));
    assert(RXNativeMethod_stackBottom != NULL);
    RXNativeMethod_stackTop = RXNativeMethod_stackBottom;
}

void RXNativeMethod_clean(void) {
    assert(RXNativeMethod_stackTop == RXNativeMethod_stackBottom);
    free(RXNativeMethod_stackBottom); // TODO remove this function when using GC
}

RXObject_t* RXNativeMethod_new(RXNativeMethodBody_t body) {
    RXObject_t* result = RXObject_allocateType(RXNativeMethod_t);
    RXObject_initialize(result);
    RXNativeMethod_body(result) = body;
    return result;
}

RXObject_t* RXNativeMethod_spawn(RXObject_t* self, RXNativeMethodBody_t body) {
    RXObject_t* result = RXNativeMethod_new(body);
    RXObject_setDelegate(result, self);
    return result;
}

RXObject_t* RXNativeMethod_activate(RXObject_t* self, RXObject_t* receiver, RXObject_t* context, int argumentCount) {
    return RXNativeMethod_body(self)(receiver, self, context, argumentCount);
}

void RXNativeMethod_push(RXObject_t* object) {
    assert(RXNativeMethod_stackTop - RXNativeMethod_stackBottom < RX_STACK_SIZE);
    *RXNativeMethod_stackTop++ = object;
}

void RXNativeMethod_pop(int offset) {
    assert(RXNativeMethod_stackTop - RXNativeMethod_stackBottom >= offset);
    RXNativeMethod_stackTop -= offset;
}

RXObject_t* RXNativeMethod_argumentAt(int index) {
    assert(RXNativeMethod_stackTop - RXNativeMethod_stackBottom > index);
    return *(RXNativeMethod_stackTop - index - 1);
}

