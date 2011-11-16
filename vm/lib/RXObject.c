
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
static RXNativeMethod_define(RXObject, spawn) {
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
static RXNativeMethod_define(RXObject, setSlot) {
    RXObject_setSlot(self, RXObject_valueOfArgumentAt(0, context), RXObject_valueOfArgumentAt(1, context));
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
static RXNativeMethod_define(RXObject, valueOfSlot) {
    return RXObject_valueOfSlot(self, RXObject_valueOfArgumentAt(0, context));
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
static RXNativeMethod_define(RXObject, deleteSlot) {
    return RXObject_deleteSlot(self, RXObject_valueOfArgumentAt(0, context));
}

/*
 * Returns a string representing the receiver.
 *
 * Arguments: none
 *
 * Returns:
 *  - a symbol
 */
static RXNativeMethod_define(RXObject, asString) {
    char* str;
    asprintf(&str, "Object_0x%" PRIxPTR, self);
    RXObject_t* result = RXSymbol_symbolForCString(str);
    free(str);
    return result;
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
static RXNativeMethod_define(RXObject, print) {
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
static RXNativeMethod_define(RXObject, println) {
    // self asString print
    RXObject_t* selfAsString = RXObject_respondTo(self, RXSymbol_asString_o, RXNil_o, 0);
    RXObject_respondTo(selfAsString, RXSymbol_println_o, RXNil_o, 0);
    return self;
}

/*
 * Return the value of the receiver in the given context.
 * This method provides a default behavior for expression evaluation.
 * See RXExpression.
 *
 * Arguments:
 *  - the evaluation context
 *
 * Returns:
 *  - self
 */
static RXNativeMethod_define(RXObject, valueInContext) {
    return self;
}

// Public --------------------------------------------------------------

RXObject_t* RXSymbol_spawn_o;
RXObject_t* RXSymbol_setSlot_o;
RXObject_t* RXSymbol_valueOfSlot_o;
RXObject_t* RXSymbol_deleteSlot_o;
RXObject_t* RXSymbol_valueInContext_o;
RXObject_t* RXSymbol_asString_o;
RXObject_t* RXSymbol_print_o;
RXObject_t* RXSymbol_println_o;
RXObject_t* RXSymbol_nil_o;

void RXObject_libSetup(void) {
    RXSymbol_spawn_o = RXSymbol_symbolForCString("spawn");
    RXSymbol_setSlot_o = RXSymbol_symbolForCString("setSlot");
    RXSymbol_valueOfSlot_o = RXSymbol_symbolForCString("valueOfSlot");
    RXSymbol_deleteSlot_o = RXSymbol_symbolForCString("deleteSlot");
    RXSymbol_valueInContext_o = RXSymbol_symbolForCString("valueInContext");
    RXSymbol_asString_o = RXSymbol_symbolForCString("asString");
    RXSymbol_print_o = RXSymbol_symbolForCString("print");
    RXSymbol_println_o = RXSymbol_symbolForCString("println");
    RXSymbol_nil_o = RXSymbol_symbolForCString("nil");
    
    RXNativeMethod_attach(RXObject, spawn);
    RXNativeMethod_attach(RXObject, setSlot);
    RXNativeMethod_attach(RXObject, valueOfSlot);
    RXNativeMethod_attach(RXObject, deleteSlot);
    RXNativeMethod_attach(RXObject, asString);
    RXNativeMethod_attach(RXObject, print);
    RXNativeMethod_attach(RXObject, println);
    RXNativeMethod_attach(RXObject, valueInContext);

    RXObject_setSlot(RXNil_o, RXSymbol_asString_o, RXSymbol_nil_o);
}

void RXObject_libClean(void) {
    // Nothing
}
