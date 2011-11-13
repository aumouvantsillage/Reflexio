
#include "RXNumber.h"
#include "RXInteger.h"

// Private -------------------------------------------------------------

static RXInteger_t* RXInteger_rawNew(int value) {
    RXInteger_t* self = (RXInteger_t*)RXCore_allocateObjectOfType(RXInteger_t);
    RXObject_initialize((RXObject_t*)self);
    self->payload = value;
    return self;
}

// Methods -------------------------------------------------------------

static RXNativeMethod_define(RXInteger, new) {
    RXInteger_t* result = RXInteger_rawNew(*(int*)self);
    RXObject_setSlot((RXObject_t*)result, RXSymbol_delegate_o, self);
    return (RXObject_t*)result;
}

static RXNativeMethod_define(RXInteger, asString) {
    char* str;
    asprintf(&str, "%d", *(int*)self);
    RXSymbol_t* result = RXSymbol_symbolForCString(str);
    free(str);
    return (RXObject_t*)result;
}

static RXNativeMethod_define(RXInteger, print) {
    printf("%d", *(int*)self);
    return self;
}

// Public --------------------------------------------------------------

RXInteger_t* RXInteger_o;

RXInteger_t* RXInteger_new(int value) {
    RXInteger_t* self = RXInteger_rawNew(value);
    RXObject_setSlot((RXObject_t*)self, RXSymbol_delegate_o, (RXObject_t*)RXInteger_o);    
    return self;
}

void RXInteger_setup(void) {
    RXInteger_o = RXInteger_rawNew(0);
    RXObject_setSlot((RXObject_t*)RXInteger_o, RXSymbol_delegate_o, RXNumber_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("Integer"), (RXObject_t*)RXInteger_o);
    
    RXNativeMethod_attach(RXInteger, new);
    RXNativeMethod_attach(RXInteger, asString);
    RXNativeMethod_attach(RXInteger, print);
}

void RXInteger_clean(void) {
    RXObject_deleteSlot((RXObject_t*)RXInteger_o, RXSymbol_delegate_o);
}
