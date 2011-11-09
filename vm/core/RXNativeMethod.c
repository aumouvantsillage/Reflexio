
#include "RXNativeMethod.h"
#include "RXSymbol.h"

RXObject_t** RXNativeMethod_stackBottom;
RXObject_t** RXNativeMethod_stackTop;

RXNativeMethod_t* RXNativeMethod_new(RXNativeMethodBody_t body) {
   RXNativeMethod_t* self = RXCore_allocateObjectOfType(RXNativeMethod_t);
   RXObject_initialize(self);
   RXObject_setIsNativeMethod(self);
   self->payload = body;
   return self;
}

static void RXNativeMethod_delete(RXNativeMethod_t* self) {
   RXObject_finalize(self);
   RXCore_deallocateObject(self);
}

void RXNativeMethod_setup(void) {
    RXNativeMethod_stackBottom = malloc(RX_STACK_SIZE * sizeof(RXObject_t*));
    assert(RXNativeMethod_stackBottom != NULL);
    RXNativeMethod_stackTop = RXNativeMethod_stackBottom;
}

void RXNativeMethod_clean(void) {
    free(RXNativeMethod_stackBottom);
}
