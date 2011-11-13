
#include "RXSymbol.h"
#include "RXObject.h"
#include <Eina.h>

// Private -------------------------------------------------------------

/*
 * A hash table of all created symbols.
 */
static Eina_Hash* RXSymbol_all;

/*
 * Core object type.
 */
typedef struct {
    RXObject_declaration;
    /* Payload: a character string
     * allocated to the appropriate size when creating a new symbol. */
    char payload[0];
} RXSymbol_t;

// Public --------------------------------------------------------------

void RXSymbol_setup(void) {
    RXSymbol_all = eina_hash_string_djb2_new(EINA_FREE_CB(RXObject_release));
}

void RXSymbol_clean(void) {
    eina_hash_free(RXSymbol_all);
}

RXObject_t* RXSymbol_new(const char* str) {
    RXObject_t* self = RXCore_allocateObjectWithSize(strlen(str) + 1);
    RXObject_initialize(self);
    RXObject_retain(self);
    strcpy((char*)self, str);
    eina_hash_direct_add(RXSymbol_all, self, self);
    return self;
}

RXObject_t* RXSymbol_symbolForCString(const char* str) {
    RXObject_t* symbol = eina_hash_find(RXSymbol_all, str);
    if(symbol == NULL) {
        symbol = RXSymbol_new(str);
        RXObject_setSlot(symbol, RXSymbol_delegate_o, RXSymbol_o);
    }
    return symbol;
}
