
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
    RXObject_t* slotName = RXExpression_valueOfArgumentAt(0, context);
    RXObject_t* value = RXExpression_valueOfArgumentAt(1, context);
    RXObject_setSlot(self, slotName, value);
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
    RXObject_t* slotName = RXExpression_valueOfArgumentAt(0, context);
    return RXObject_valueOfSlot(self, slotName);
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
    RXObject_t* slotName = RXExpression_valueOfArgumentAt(0, context);
    return RXObject_deleteSlot(self, slotName);
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

// Public --------------------------------------------------------------

RXObject_t* RXLobby_o;
RXObject_t* RXSymbol_Lobby_o;
RXObject_t* RXSymbol_Object_o;
RXObject_t* RXSymbol_spawn_o;
RXObject_t* RXSymbol_setSlot_o;
RXObject_t* RXSymbol_valueOfSlot_o;
RXObject_t* RXSymbol_deleteSlot_o;
RXObject_t* RXSymbol_asString_o;
RXObject_t* RXSymbol_asSource_o;
RXObject_t* RXSymbol_print_o;
RXObject_t* RXSymbol_println_o;
RXObject_t* RXSymbol_nil_o;

void RXObject_libSetup(void) {
    RXSymbol_Lobby_o = RXSymbol_symbolForCString("Lobby");
    RXSymbol_Object_o = RXSymbol_symbolForCString("Object");
    RXSymbol_spawn_o = RXSymbol_symbolForCString("spawn");
    RXSymbol_setSlot_o = RXSymbol_symbolForCString("setSlot");
    RXSymbol_valueOfSlot_o = RXSymbol_symbolForCString("valueOfSlot");
    RXSymbol_deleteSlot_o = RXSymbol_symbolForCString("deleteSlot");
    RXSymbol_asString_o = RXSymbol_symbolForCString("asString");
    RXSymbol_asSource_o = RXSymbol_symbolForCString("asSource");
    RXSymbol_print_o = RXSymbol_symbolForCString("print");
    RXSymbol_println_o = RXSymbol_symbolForCString("println");
    RXSymbol_nil_o = RXSymbol_symbolForCString("nil");
    
    RXNativeMethod_attach(RXObject, spawn);
    RXNativeMethod_attach(RXObject, setSlot);
    RXNativeMethod_attach(RXObject, valueOfSlot);
    RXNativeMethod_attach(RXObject, deleteSlot);
    RXObject_setSlot(RXObject_o, RXSymbol_asSource_o,
        RXNativeMethod_attach(RXObject, asString)
    );
    RXNativeMethod_attach(RXObject, print);
    RXNativeMethod_attach(RXObject, println);

    RXObject_setSlot(RXNil_o, RXSymbol_asString_o, RXSymbol_nil_o);
    
    RXLobby_o = RXObject_spawn(RXObject_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_Lobby_o, RXLobby_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_Object_o, RXObject_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_nil_o, RXNil_o);
}

void RXObject_libClean(void) {
    // Nothing
}
