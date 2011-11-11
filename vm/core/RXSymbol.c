
#include "RXSymbol.h"
#include "RXObject.h"
#include "RXNativeMethod.h"

#include <Eina.h>

// Private -------------------------------------------------------------

/*
 * A hash table of all created symbols.
 */
static Eina_Hash* RXSymbol_all;

/*
 * Core object type.
 */
struct RXSymbol_s {
    RXObject_declaration;
    /* Payload: a character string
     * allocated to the appropriate size when creating a new symbol. */
    char payload[0];
};

// Public --------------------------------------------------------------

void RXSymbol_setup(void) {
    RXSymbol_all = eina_hash_string_djb2_new(EINA_FREE_CB(RXObject_release));
}

void RXSymbol_clean(void) {
    eina_hash_free(RXSymbol_all);
}

RXSymbol_t* RXSymbol_new(const char* str) {
    RXSymbol_t* self = (RXSymbol_t*)RXCore_allocateObjectWithSize(strlen(str) + 1);
    RXObject_initialize((RXObject_t*)self);
    RXObject_retain((RXObject_t*)self);
    strcpy(self->payload, str);
    eina_hash_direct_add(RXSymbol_all, self->payload, self);
    return self;
}

RXSymbol_t* RXSymbol_symbolForCString(const char* str) {
    RXSymbol_t* symbol = eina_hash_find(RXSymbol_all, str);
    if(symbol == NULL) {
        symbol = RXSymbol_new(str);
        RXObject_setSlot((RXObject_t*)symbol, RXSymbol_delegate_o, (RXObject_t*)RXSymbol_o);
    }
    return symbol;
}
