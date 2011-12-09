
#include "RXCore.h"
#include <Eina.h>

// Private--------------------------------------------------------------

#ifndef RX_CACHE_ENABLE
#warning Flag RX_CACHE_ENABLE not defined: method cache is disabled.
#endif

/*
 * A virtual table is an object containing a set
 * of associations between names and methods.
 * Associations are implemented as a red-black tree.
 */
RXObject_defineType(VTable,
    Eina_Rbtree* methods;
);

/*
 * An entry of a VTable contains references to a name object and a method object.
 * An entry is stored as a node in a red-black tree.
 */
typedef struct {
    EINA_RBTREE;
    RXObject_t* name;
    RXObject_t* method;
} RXVTableEntry_t;

/*
 * Direct access to the "methods" field of a VTable.
 */
#define RXVTable_methods(self) ((RXVTable_t*)self)->methods

/*
 * Name comparison callback used during method lookup.
 */
static inline int RXVTable_compareNames(const RXVTableEntry_t *entry, const RXObject_t *name, int length, void *data) {
    return (int)((intptr_t)entry->name - (intptr_t)name);
}

/*
 * Entry comparison callback used when adding a method to a VTable.
 */
static Eina_Rbtree_Direction RXVTable_compareEntries(const RXVTableEntry_t *left, const RXVTableEntry_t *right, RXObject_t *name) {
    return RXVTable_compareNames(left, name, 0, NULL) < 0
        ? EINA_RBTREE_LEFT
        : EINA_RBTREE_RIGHT; 
}

/*
 * Return the entry for the given name in the current VTable.
 */
static inline RXVTableEntry_t* RXVTable_entryForName(const RXObject_t* self, const RXObject_t* name) {
    return (RXVTableEntry_t*)eina_rbtree_inline_lookup(RXVTable_methods(self), name, 0, EINA_RBTREE_CMP_KEY_CB(RXVTable_compareNames), NULL);
}

// Public --------------------------------------------------------------

RXObject_t* RXVTable_new(void) {
    RXObject_t* result = RXObject_allocateType(RXVTable_t);
    RXVTable_methods(result) = NULL;
    RXObject_vtable(result) = RXVTable_o;
    return result;
}

// TODO inline this function
void RXVTable_delete(RXObject_t* self) {
    eina_rbtree_delete(RXVTable_methods(self), EINA_RBTREE_FREE_CB(free), NULL);
}

void RXVTable_setMethodForName(RXObject_t* self, RXObject_t* name, RXObject_t* method) {
    RXVTableEntry_t* entry = RXVTable_entryForName(self, name);
    if (entry == NULL) {
        entry = malloc(sizeof(RXVTableEntry_t));
        RXVTable_methods(self) = eina_rbtree_inline_insert(RXVTable_methods(self), (Eina_Rbtree*)entry, EINA_RBTREE_CMP_NODE_CB(RXVTable_compareEntries), name);
        entry->name = name;
    }
    entry->method = method;
    // TODO remove cache entry
}

RXObject_t* RXVTable_methodForName(const RXObject_t* self, const RXObject_t* name) {
    RXVTableEntry_t* entry = RXVTable_entryForName(self, name);
    if (entry != NULL) {
        return entry->method;
    }
    else {
        // TODO lookup in cache
        // TODO send lookup message
        // TODO add cache entry
        return RXNativeMethod_methodNotFound_o;
    }
}

RXObject_t* RXVTable_removeMethodForName(RXObject_t* self, const RXObject_t* name) {
    RXVTableEntry_t* entry = RXVTable_entryForName(self, name);
    if (entry != NULL) {
        RXObject_t* method = entry->method;
        RXVTable_methods(self) = eina_rbtree_inline_remove(RXVTable_methods(self), (Eina_Rbtree*)entry, EINA_RBTREE_CMP_NODE_CB(RXVTable_compareEntries), NULL);
        free(entry);
        // TODO remove cache entry
    }
    else {
        return RXNativeMethod_methodNotFound_o;
    }
}
