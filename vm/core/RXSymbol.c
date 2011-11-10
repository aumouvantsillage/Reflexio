
#include "RXSymbol.h"
#include "RXObject.h"
#include "RXNativeMethod.h"

#include <Eina.h>

// Private -------------------------------------------------------------

static Eina_Hash* RXSymbol_all;

/*
 * Core object type.
 */
typedef struct RXSymbol_s {
    RXObject_declaration;
    /* Payload: a character string
     * allocated to the appropriate size when creating a new symbol. */
    char payload[0];
} RXSymbol_t;

/*
 * Print a hash table entry with the given key and data
 */
static Eina_Bool RXSymbol_dump(const Eina_Hash* hash, const char* key, const char* data, void* fdata) {
    printf("%s => %s\n", key, data);
    return 1;
}

// Public --------------------------------------------------------------

void RXSymbol_setup(void) {
    RXSymbol_all = eina_hash_string_djb2_new(NULL);
}

void RXSymbol_clean(void) {
    eina_hash_free(RXSymbol_all);
}

RXSymbol_t* RXSymbol_new(const char* str) {
    RXSymbol_t* self = (RXSymbol_t*)RXCore_allocateObjectWithSize(strlen(str) + 1);
    RXObject_initialize((RXObject_t*)self);
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

void RXSymbol_dumpAll(void) {
    printf("%d symbols\n", eina_hash_population(RXSymbol_all));
    eina_hash_foreach(RXSymbol_all, (Eina_Hash_Foreach)RXSymbol_dump, NULL);
}
