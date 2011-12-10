
#include "RXCore.h"

// Private--------------------------------------------------------------

RXObject_defineType(RXSlot_t,
    RXNativeMethodBody_t getter;
    RXObject_t* value;
);

#define RXSlot_getter(self) ((RXSlot_t*)self)->getter
#define RXSlot_value(self) ((RXSlot_t*)self)->value

RXNativeMethod_define(Slot, value) {
    return RXSlot_value(self);
}

// Public --------------------------------------------------------------

RXObject_t* RXSlot_o;

RXObject_t* RXSlot_new(RXObject_t* value) {
    RXObject_t* result = RXObject_allocateType(RXSlot_t);
    RXObject_initialize(result);
    RXSlot_getter(result) = RXNativeMethod_functionName(Slot, value);
    RXSlot_value(result) = value;
    return result;
}

RXObject_t* RXSlot_spawn(RXObject_t* self, RXObject_t* value) {
    RXObject_t* result = RXSlot_new(value);
    RXObject_setDelegate(result, self);
    return result;
}
