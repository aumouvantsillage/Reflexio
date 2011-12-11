
#include "RXLib.h"
#include <inttypes.h>

// Methods -------------------------------------------------------------

/*
 * Return a new object with self as delegate.
 *
 * Arguments: none
 *
 * Returns:
 *  - a new object
 */
RXNativeMethod_define(RXProtoObject, spawn) {
    return RXObject_spawn(self);
}

/*
 * Assign a slot of the receiver.
 *
 * Arguments:
 *  - slot name (a symbol)
 *  - the object to assign
 *
 * Returns:
 *  - self
 */
RXNativeMethod_define(RXProtoObject, setSlot) {
    if (argumentCount > 0) {
        RXObject_t* slotName = RXExpression_valueOfArgumentAt(0, context);
        RXObject_t* value = argumentCount > 1
            ? RXExpression_valueOfArgumentAt(1, context)
            : RXNil_o;
        RXObject_setSlot(self, slotName, value);
    }
    return self;
}

/*
 * Return the value of a slot in the receiver.
 *
 * Arguments:
 *  - slot name (a symbol)
 *
 * Returns:
 *  - the value of the slot with the given name
 */
RXNativeMethod_define(RXProtoObject, valueOfSlot) {
    if (argumentCount > 0) {
        RXObject_t* slotName = RXExpression_valueOfArgumentAt(0, context);
        return RXObject_valueOfSlot(self, slotName);
    }
    else {
        return RXNil_o;
    }
}

/*
 * Remove a slot and return the value of that slot.
 *
 * Arguments:
 *  - slot name (a symbol)
 *
 * Returns:
 *  - the value of the slot before it was deleted
 */
RXNativeMethod_define(RXProtoObject, deleteSlot) {
    if (argumentCount > 0) {
        RXObject_t* slotName = RXExpression_valueOfArgumentAt(0, context);
        return RXObject_deleteSlot(self, slotName);
    }
    else {
        return RXNil_o;
    }
}

/*
 * Returns a string representing the receiver.
 *
 * Arguments: none
 *
 * Returns:
 *  - a symbol
 */
RXNativeMethod_define(RXProtoObject, asString) {
    char* str;
    asprintf(&str, "Object_0x%" PRIxPTR, self);
    RXObject_t* result = RXSymbol_symbolForCString(str);
    free(str);
    return result;
}

/*
 * Returns a string representing the receiver as source code.
 * The default behavior is to return the result of asString on
 * the receiver.
 *
 * Arguments: none
 *
 * Returns:
 *  - a symbol
 */
RXNativeMethod_define(RXProtoObject, asSource) {
    return RXObject_respondTo(self, RXSymbol_asString_o, context, argumentCount);
}

/*
 * Print the receiver on the standard output.
 * This method calls asString to get a representation of the receiver.
 *
 * Arguments: none
 *
 * Returns:
 *  - self
 */
RXNativeMethod_define(RXProtoObject, print) {
    // self asString print
    RXObject_t* selfAsString = RXObject_respondTo(self, RXSymbol_asString_o, RXNil_o, 0);
    RXObject_respondTo(selfAsString, RXSymbol_print_o, RXNil_o, 0);
    return self;
}

/*
 * Print the receiver on the standard output and append a new line.
 * This method calls asString to get a representation of the receiver.
 *
 * Arguments: none
 *
 * Returns:
 *  - self
 */
RXNativeMethod_define(RXProtoObject, println) {
    // self asString print
    RXObject_t* selfAsString = RXObject_respondTo(self, RXSymbol_asString_o, RXNil_o, 0);
    RXObject_respondTo(selfAsString, RXSymbol_println_o, RXNil_o, 0);
    return self;
}

RXNativeMethod_define(RXProtoObject, respondTo) {
    RXObject_t* messageName = argumentCount > 0
        ? RXExpression_valueOfArgumentAt(0, context)
        : RXSymbol_nil_o;
    fprintf(stderr, "No method found for message \"%s\"\n", messageName);
    exit(EXIT_FAILURE);
}

// Public --------------------------------------------------------------

RXObject_t* RXSymbol_spawn_o;
RXObject_t* RXSymbol_setSlot_o;
RXObject_t* RXSymbol_valueOfSlot_o;
RXObject_t* RXSymbol_deleteSlot_o;
RXObject_t* RXSymbol_asString_o;
RXObject_t* RXSymbol_asSource_o;
RXObject_t* RXSymbol_print_o;
RXObject_t* RXSymbol_println_o;

void RXObject_libSetup(void) {
    RXSymbol_spawn_o = RXSymbol_symbolForCString("spawn");
    RXSymbol_setSlot_o = RXSymbol_symbolForCString("setSlot");
    RXSymbol_valueOfSlot_o = RXSymbol_symbolForCString("valueOfSlot");
    RXSymbol_deleteSlot_o = RXSymbol_symbolForCString("deleteSlot");
    RXSymbol_asString_o = RXSymbol_symbolForCString("asString");
    RXSymbol_asSource_o = RXSymbol_symbolForCString("asSource");
    RXSymbol_print_o = RXSymbol_symbolForCString("print");
    RXSymbol_println_o = RXSymbol_symbolForCString("println");
    
    RXNativeMethod_attach(RXProtoObject, spawn);
    RXNativeMethod_attach(RXProtoObject, setSlot);
    RXNativeMethod_attach(RXProtoObject, valueOfSlot);
    RXNativeMethod_attach(RXProtoObject, deleteSlot);
    RXNativeMethod_attach(RXProtoObject, asString);
    RXNativeMethod_attach(RXProtoObject, asSource);
    RXNativeMethod_attach(RXProtoObject, print);
    RXNativeMethod_attach(RXProtoObject, println);
    RXNativeMethod_attach(RXProtoObject, respondTo);
}

void RXObject_libClean(void) {
    // Nothing
}
