
#include "RXSymbol.h"

// Private -------------------------------------------------------------

static Eina_Hash* RXSymbol_all;

/*
 * Core object type.
 */
typedef struct RXSymbol_s {
    RX_OBJECT;
    /* Payload: a character string
     * allocated to the appropriate size when creating a new symbol. */
    char payload[0];
} RXSymbol_t;


// Public --------------------------------------------------------------

void RXSymbol_setup(void) {
    RXSymbol_all = eina_hash_string_djb2_new(NULL); // TODO Add callback for object removal
}

RXSymbol_t* RXSymbol_symbolForCString(const char* str) {
    RXSymbol_t* symbol = eina_hash_find(RXSymbol_all, str);
    if(symbol == NULL) {
        symbol = RXCore_allocateObjectWithSize(strlen(str) + 1);
        RXObject_initialize(symbol);
        strcpy(symbol->payload, str);
        eina_hash_direct_add(RXSymbol_all, symbol->payload, symbol);
    }
    return symbol;
}

static Eina_Bool RXSymbol_dump(const Eina_Hash* hash, const char* key, const char* data, void* fdata) {
    printf("%s => %s\n", key, data);
    return 1;
}

void RXSymbol_dumpAll(void) {
    printf("%d symbols\n", eina_hash_population(RXSymbol_all));
    eina_hash_foreach(RXSymbol_all, (Eina_Hash_Foreach)RXSymbol_dump, NULL);
}
