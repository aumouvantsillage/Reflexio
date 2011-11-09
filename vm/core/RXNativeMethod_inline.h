
#include <assert.h>

inline static RXObject_t* RXNativeMethod_activate(RXNativeMethod_t* self, RXObject_t* receiver) {
    return self->payload(receiver);
}

extern RXObject_t** RXNativeMethod_stackBottom;
extern RXObject_t** RXNativeMethod_stackTop;

#ifndef RX_STACK_SIZE
#define RX_STACK_SIZE 1024
#endif

inline static void RXNativeMethod_push(RXObject_t* object) {
    assert(RXNativeMethod_stackBottom - RXNativeMethod_stackTop < RX_STACK_SIZE);
    *RXNativeMethod_stackTop++ = object;
}

inline static void RXNativeMethod_pop(int offset) {
    assert(RXNativeMethod_stackBottom - RXNativeMethod_stackTop >= offset);
    RXNativeMethod_stackTop -= offset;
}

inline static RXObject_t* RXNativeMethod_argumentAt(int index) {
    assert(RXNativeMethod_stackBottom - RXNativeMethod_stackTop >= index);
    return *(RXNativeMethod_stackTop - index);
}
