
#include "RXCore.h"

void RXObject_initialize(RXObject_t* self) {
    RXObject_vtable(self) = RXVTable_new();
}

RXObject_t* RXObject_new(void) {
    RXObject_t* result = RXObject_allocateType(RXObject_t);
    RXObject_initialize(result);
    return result;
}

RXObject_t* RXObject_spawn(RXObject_t* self) {
    RXObject_t* result = RXObject_new();
    RXObject_setDelegate(result, self);
    return result;
}

RXObject_t* RXObject_allocateSize(const size_t size) {
    return (RXObject_t*)((char*)malloc(size + sizeof(RXObject_t*)) + sizeof(RXObject_t*));
}

void RXObject_deallocate(RXObject_t* self) {
    free((char*)self - sizeof(RXObject_t*));
}

void RXObject_finalize(RXObject_t* self) {
    RXVTable_delete(RXObject_vtable(self));
}

void RXObject_delete(RXObject_t* self) {
    RXObject_finalize(self);
    RXObject_deallocate(self);
}

void RXObject_setSlot(RXObject_t* self, RXObject_t* name, RXObject_t* value) {
    RXVTable_setMethodForName(RXObject_vtable(self), name, RXSlot_spawn(RXSlot_o, value));
}

void RXObject_setDelegate(RXObject_t* self, RXObject_t* delegate) {
    RXObject_setSlot(self, RXSymbol_delegate_o, delegate);
}

RXObject_t* RXObject_respondTo(RXObject_t* self, RXObject_t* messageName, RXObject_t* context, int argumentCount) {
    RXObject_t* method = RXVTable_methodForName(RXObject_vtable(self), messageName);
    RXObject_t* result = RXNativeMethod_activate(method, self, context, argumentCount);
    RXNativeMethod_pop(argumentCount);
    return result;
}
