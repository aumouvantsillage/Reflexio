
#ifndef __RX_METHOD_H__
#define __RX_METHOD_H__

#include "RXObject.h"

/*
 * Function type for native method bodies.
 */
typedef RXObject_t* (*RXMethodBody_t) (RXObject_t* self, RXObject_t* context, int argumentCount, RXObject_t* arguments);

/*
 * A native method is an object with a reference to a C function.
 */
RXObject_defineType(RXMethod_t, RXMethodBody_t);

/*
 * The standard function name for methods attached to predefined object types.
 */
#define RXMethod_functionName(type, name) \
    type##_respondTo##_##name

/*
 * The standard C function prototype for methods attached to predefined object types.
 *
 * Example:
 *
 * RXMethod_define(MyType, someMessage) {
 *     // Method body
 * }
 *
 * The static keyword can be added before RXMethod_define.
 */
#define RXMethod_define(type, name) \
    RXObject_t* RXMethod_functionName(type, name)( \
        RXObject_t* self, RXObject_t* context, int argumentCount, RXObject_t* arguments)

/*
 * Attach a method to an object representing the given type.
 *
 * If type is MyType, we assume that there exists a variable MyType_o of type MyType_t*
 *
 * Example:
 *
 * MyType_t* MyType_o = MyType_new();
 * RXMethod_attach(MyType, someMessage)
 */
#define RXMethod_attach(type, name) \
    RXObject_setSlot((RXObject_t*)type##_o, RXSymbol_symbolForCString(#name), (RXObject_t*)RXMethod_new(RXMethod_functionName(type, name)))

/*
 * Create a new native method with the given body.
 *
 * Use macro RXMethod_attach to create a method and attach it to an object.
 */
RXMethod_t* RXMethod_new(RXMethodBody_t body);

#define RXMethod_activate(self, receiver, context, argumentCount, arguments) \
    ((RXMethod_t*)self)->payload((receiver), (context), (argumentCount), (arguments))

#endif
