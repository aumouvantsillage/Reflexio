
#ifndef __RX_CORE_NATIVE_METHOD_H__
#define __RX_CORE_NATIVE METHOD_H__

#include "RXObject.h"

/*
 * Create objects managed by this module.
 */
void RXNativeMethod_setup(void);

/*
 * Delete objects managed by this module.
 */
void RXNativeMethod_clean(void);

/*
 * The C function implementing a native method has the following parameters:
 *  - self: the receiver of the message
 *  - method: the method object being activated
 *  - context: the object where context data is available for argument evaluation
 *  - argumentCount: the number of arguments passed to the method
 *
 * Message arguments are passed on a stack.
 *
 * TODO add the stack as a parameter
 */
#define RXNativeMethodBody_parameters \
    (RXObject_t* self, RXObject_t* method, RXObject_t* context, int argumentCount)

/*
 * Function type for native method bodies.
 */
typedef RXObject_t* (*RXNativeMethodBody_t) RXNativeMethodBody_parameters;

RXObject_t* RXNativeMethod_new(RXNativeMethodBody_t body);

RXObject_t* RXNativeMethod_spawn(RXObject_t* self, RXNativeMethodBody_t body);

/*
 * Execute a given native method on the given receiver.
 */
RXObject_t* RXNativeMethod_activate(RXObject_t* self, RXObject_t* receiver, RXObject_t* context, int argumentCount);

/*
 * The standard function name for methods attached to predefined object types.
 */
#define RXNativeMethod_functionName(type, name) \
    RX##type##_respondTo##_##name

/*
 * The standard C function prototype for methods attached to predefined object types.
 *
 * Example:
 *
 * RXNativeMethod_define(MyType, someMessage) {
 *     // Method body
 * }
 *
 * The static keyword can be added before RXNativeMethod_define.
 */
#define RXNativeMethod_define(type, name) \
    static RXObject_t* RXNativeMethod_functionName(type, name) RXNativeMethodBody_parameters

/*
 * Attach a method to an object representing the given type.
 *
 * If type is MyType, we assume that there exists a variable MyType_o of type MyType_t*
 *
 * Example:
 *
 * MyType_t* MyType_o = MyType_new();
 * RXNativeMethod_attach(MyType, someMessage)
 */
#define RXNativeMethod_attach(type, name) ({ \
    RXObject_t* method = RXNativeMethod_spawn(RXNativeMethod_o, RXNativeMethod_functionName(type, name)); \
    RXVTable_setMethodForName(RXObject_vtable(type##_o), RXSymbol_symbolForCString(#name), method); \
    method; \
})

/*
 * Push an argument to the stack.
 */
void RXNativeMethod_push(RXObject_t* object);

/*
 * Remove a given number of arguments from the stack.
 */
void RXNativeMethod_pop(int offset);

/*
 * Read the argument at the given index.
 */
RXObject_t* RXNativeMethod_argumentAt(int index);

#endif
