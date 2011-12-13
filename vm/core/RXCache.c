
#ifdef RX_CACHE_ENABLE

#include "RXCore.h"
#include <Eina.h>

// Private -------------------------------------------------------------

static Eina_Hash* RXCache_data;

// Public --------------------------------------------------------------

void RXCache_setup(void) {
    RXCache_data = eina_hash_pointer_new(EINA_FREE_CB(free));
}

RXCacheVersion_t RXCache_addSlotName(RXObject_t* slotName) {
    RXCacheVersion_t* version = eina_hash_find(RXCache_data, &slotName);
    if (version == NULL) {
        version = malloc(sizeof(RXCacheVersion_t));
        *version = 0;
        eina_hash_add(RXCache_data, &slotName, version);
    }
    return *version;
}

void RXCache_setDirty(RXObject_t* slotName) {
    RXCacheVersion_t* version = eina_hash_find(RXCache_data, &slotName);
    if (version != NULL) {
        (*version)++;
    }
}

RXCacheVersion_t RXCache_version(RXObject_t* slotName) {
    return *(RXCacheVersion_t*)eina_hash_find(RXCache_data, &slotName);
}

void RXCache_removeSlotName(RXObject_t* slotName) {
    eina_hash_del_by_key(RXCache_data, &slotName);
}

void RXCache_clean(void) {
    eina_hash_free(RXCache_data);
}

#endif
