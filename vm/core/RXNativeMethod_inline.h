
#include <assert.h>

/*
 * Execute a given native method on the given receiver.
 */
inline static RXObject_t* RXNativeMethod_activate(RXNativeMethod_t* self, RXObject_t* receiver) {
    return self->payload(receiver);
}

/*
 * Method argument stack.
 */
extern RXObject_t** RXNativeMethod_stackBottom;
extern RXObject_t** RXNativeMethod_stackTop;

/*
 * Override constant RX_STACK_SIZE to change the stack size.
 */
#ifndef RX_STACK_SIZE
#define RX_STACK_SIZE 1024
#endif

/*
 * Push an argument to the stack.
 */
inline static void RXNativeMethod_push(RXObject_t* object) {
    assert(RXNativeMethod_stackTop - RXNativeMethod_stackBottom < RX_STACK_SIZE);
    *RXNativeMethod_stackTop++ = object;
}

/*
 * Remove a given number of arguments from the stack.
 */
inline static void RXNativeMethod_pop(int offset) {
    assert(RXNativeMethod_stackTop - RXNativeMethod_stackBottom >= offset);
    RXNativeMethod_stackTop -= offset;
}

/*
 * Read the argument at the given index.
 */
inline static RXObject_t* RXNativeMethod_argumentAt(int index) {
    assert(RXNativeMethod_stackTop - RXNativeMethod_stackBottom > index);
    return *(RXNativeMethod_stackTop - index - 1);
}
