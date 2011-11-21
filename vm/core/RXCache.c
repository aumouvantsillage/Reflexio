
#include "RXCore.h"
#include <Eina.h>
#include <inttypes.h>

// Private -------------------------------------------------------------

static Eina_Hash* RXCache_data;

// Public --------------------------------------------------------------

void RXCache_setup(void) {
    RXCache_data = eina_hash_pointer_new(EINA_FREE_CB(eina_hash_free));
}

void RXCache_addEntry(RXObject_t* object, RXObject_t* slotName, RXObject_t* value) {
    Eina_Hash* sub = eina_hash_find(RXCache_data, slotName);
    if (sub == NULL) {
        sub = eina_hash_pointer_new(NULL);
        eina_hash_add(RXCache_data, &slotName, sub);
    }
    eina_hash_add(sub, &object, value);
}

void RXCache_removeSlotName(RXObject_t* slotName) {
    eina_hash_del_by_key(RXCache_data, &slotName);
}

RXObject_t* RXCache_valueForEntry(RXObject_t* object, RXObject_t* slotName) {
    RXObject_t* result = RXNil_o;
    Eina_Hash* sub = eina_hash_find(RXCache_data, &slotName);
    if (sub != NULL) {
        result = eina_hash_find(sub, &object);
        if (result == NULL) {
            result = RXNil_o;
        }
    }
    return result;
}

void RXCache_clean(void) {
    eina_hash_free(RXCache_data);
}
