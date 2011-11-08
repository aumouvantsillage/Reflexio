
#include "RXMethod.h"
#include "RXSymbol.h"

RXMethod_t* RXMethod_new(RXMethodBody_t body) {
   RXMethod_t* self = RXCore_allocateObjectOfType(RXMethod_t);
   RXObject_initialize(self);
   RXObject_setIsNativeMethod(self);
   self->payload = body;
   return self;
}

static void RXMethod_delete(RXMethod_t* self) {
   RXObject_finalize(self);
   RXCore_deallocateObject(self);
}

static RXMethod_define(RXMethod, null) {
   return self;
}
