
#ifndef __RX_NATIVE_METHOD_H__
#define __RX_NATIVE METHOD_H__

#include "RXObject.h"

/*
 * Function type for native method bodies.
 */
typedef RXObject_t* (*RXNativeMethodBody_t) (RXObject_t* self);

/*
 * A native method is an object with a reference to a C function.
 */
RXObject_defineType(RXNativeMethod_t, RXNativeMethodBody_t);

extern RXNativeMethod_t* RXNativeMethod_o;

/*
 * The standard function name for methods attached to predefined object types.
 */
#define RXNativeMethod_functionName(type, name) \
    type##_respondTo##_##name

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
    RXObject_t* RXNativeMethod_functionName(type, name) (RXObject_t* self)

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
    RXNativeMethod_t* method = RXNativeMethod_new(RXNativeMethod_functionName(type, name)); \
    RXObject_setSlot((RXObject_t*)type##_o, RXSymbol_symbolForCString(#name), (RXObject_t*)method); \
    RXObject_setSlot((RXObject_t*)method, RXSymbol_delegate_o, (RXObject_t*)RXNativeMethod_o); \
    method; \
})

void RXNativeMethod_setup(void);

void RXNativeMethod_clean(void);

/*
 * Create a new native method with the given body.
 *
 * Use macro RXNativeMethod_attach to create a method and attach it to an object.
 */
RXNativeMethod_t* RXNativeMethod_new(RXNativeMethodBody_t body);

#include "RXNativeMethod_inline.h"

#endif
