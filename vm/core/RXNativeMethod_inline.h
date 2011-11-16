
#ifndef __RX_CORE_NATIVE_METHOD_INLINE_H__
#define __RX_CORE_NATIVE_METHOD_INLINE_H__

#include "RXNativeMethod.h"
#include <assert.h>

inline static bool RXObject_isNativeMethod(const RXObject_t* self) {
    return (RXObject_coreData(self).flags & RXObject_flagIsNativeMethod) != 0;
}

inline static void RXObject_setIsNativeMethod(RXObject_t* self) { 
    RXObject_coreData(self).flags |= RXObject_flagIsNativeMethod;
}

/*
 * Execute a given native method on the given receiver.
 */
inline static RXObject_t* RXNativeMethod_activate(RXObject_t* self, RXObject_t* receiver, RXObject_t* context, int argumentCount) {
    return (*(RXNativeMethodBody_t*)self)(receiver, context, argumentCount);
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

#endif
