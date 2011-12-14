
#include "RXCore.h"
#include <inttypes.h>

// Private -------------------------------------------------------------

#ifdef RX_CACHE_ENABLE
    static Eina_Hash* RXCache_data;
    typedef unsigned RXCacheVersion_t;
#else
    #warning Flag RX_CACHE_ENABLE not defined: method cache is disabled.
#endif

/*
 * Node type for Red-Black binary trees containing object slots.
 */
typedef struct {
    EINA_RBTREE;
    RXObject_t* key;
    RXObject_t* value;
#ifdef RX_CACHE_ENABLE
    bool cached;
    RXCacheVersion_t version;
#endif
} RXObjectNode_t;

RX_INLINE bool RXObject_isLookingUp(const RXObject_t* self) {
    return (RXObject_coreData(self).flags & RXObject_flagIsLookingUp) != 0;
}

RX_INLINE void RXObject_setIsLookingUp(RXObject_t* self) { 
    RXObject_coreData(self).flags |= RXObject_flagIsLookingUp;
}

RX_INLINE void RXObject_clearIsLookingUp(RXObject_t* self) {
    RXObject_coreData(self).flags &= ~RXObject_flagIsLookingUp;
}

/*
 * Key comparison callback used during slot lookup.
 */
static int RXObject_compareKeys(const RXObjectNode_t *node, const RXObject_t *key, int length, void *data) {
    return (int)((intptr_t)node->key - (intptr_t)key);
}

/*
 * Node comparison callback used diring slot assignment.
 */
static Eina_Rbtree_Direction RXObject_compareNodes(const RXObjectNode_t *left, const RXObjectNode_t *right, RXObject_t *slotName) {
    return (RXObject_compareKeys(left, slotName, 0, NULL) < 0)
        ? EINA_RBTREE_LEFT
        : EINA_RBTREE_RIGHT; 
}

RX_INLINE RXObjectNode_t* RXObject_node(const RXObject_t* self, const RXObject_t* slotName) {
    return (RXObjectNode_t*)eina_rbtree_inline_lookup(RXObject_coreData(self).slots, slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
}

#ifdef RX_CACHE_ENABLE
RX_INLINE RXCacheVersion_t RXCache_addSlotName(RXObject_t* slotName) {
    RXCacheVersion_t* version = eina_hash_find(RXCache_data, &slotName);
    if (version == NULL) {
        version = malloc(sizeof(RXCacheVersion_t));
        *version = 0;
        eina_hash_add(RXCache_data, &slotName, version);
    }
    return *version;
}

RX_INLINE RXCacheVersion_t RXCache_version(RXObject_t* slotName) {
    return *(RXCacheVersion_t*)eina_hash_find(RXCache_data, &slotName);
}

RX_INLINE void RXCache_setDirty(RXObject_t* slotName) {
    RXCacheVersion_t* version = eina_hash_find(RXCache_data, &slotName);
    if (version != NULL) {
        (*version)++;
    }
}

RX_INLINE void RXObject_setDirty(const RXObject_t* self) {
    Eina_Iterator* iter = eina_rbtree_iterator_prefix(RXObject_coreData(self).slots);
    RXObjectNode_t* node;
    while(eina_iterator_next(iter, (void**)&node)) {
        if (node->cached) {
            RXCache_setDirty(node->key);
        }
    }
}

RX_INLINE void RXCache_removeSlotName(RXObject_t* slotName) {
    eina_hash_del_by_key(RXCache_data, &slotName);
}
#endif

// Public --------------------------------------------------------------

void RXObject_setSlot(RXObject_t* self, RXObject_t* slotName, RXObject_t* value, bool cached) {
    RXObjectNode_t* node = RXObject_node(self, slotName);
    
#ifdef RX_CACHE_ENABLE
    // Modifying the lookup method invalidates all cached slots
    // from the current object 
    if (!cached) {
        if (slotName == RXSymbol_lookup_o) {
            RXObject_setDirty(self);
        }
        
        // Modifying a non-cached slot invalidates all cached slots with
        // the same name in all other objects.
        if (node != NULL) {
            RXCache_setDirty(slotName);
        }
    }
#endif

    // Create a new node if none was found for the given name
    if (node == NULL) {
        node = malloc(sizeof(RXObjectNode_t));
        RXObject_coreData(self).slots = eina_rbtree_inline_insert(
            RXObject_coreData(self).slots,
            (Eina_Rbtree*)node,
            EINA_RBTREE_CMP_NODE_CB(RXObject_compareNodes),
            slotName);
        node->key = slotName;
    }
    
    node->value = value;
    
#ifdef RX_CACHE_ENABLE
    node->cached = cached;
    if (cached) {
        node->version = RXCache_addSlotName(slotName);
    }
#endif
}

void RXObject_setDelegate(RXObject_t* self, RXObject_t* delegate) {
#ifdef RX_CACHE_ENABLE
    // Modifying the delegate invalidates all cached slots from the current object
    if (RXObject_delegate(self) != NULL) {
        RXObject_setDirty(self);
    }
#endif
    // TODO prevent cycles in the delegate chain when changing an already assigned delegate
    RXObject_coreData(self).delegate = (RXObject_t*)((intptr_t)delegate | RXObject_coreData(self).flags & 3);
}

/*
 * Default slot lookup method.
 *
 * If the receiver has a slot with the given name, return the value of that slot.
 * If a lookup method exists in the receiver, send an "activate" message to that method and return the result.
 * If the receiver has a "delegate" slot, recursively look up in the delegate.
 */
RXObject_t* RXObject_valueOfSlot(RXObject_t* self, RXObject_t* slotName) {
    RXObject_t* result = RXNil_o;

    // Look up in the own slots of the current object.
    // If cache is enable, return non-dirty slot values only.
    RXObjectNode_t* node = RXObject_node(self, slotName);
    if (node != NULL
#ifdef RX_CACHE_ENABLE
        && (!node->cached || node->cached && node->version == RXCache_version(slotName))
#endif
    ) {
        return node->value;
    }

    if (result == RXNil_o && slotName != RXSymbol_lookup_o && !RXObject_isLookingUp(self)) {
        // If a lookup method exists in the receiver'delegate chain,
        // send an "activate" message to that method
        RXObject_t* lookupMethod = RXObject_valueOfSlot(self, RXSymbol_lookup_o);
        if (lookupMethod != RXNil_o) {
            // lookupMethod activate(self, slotName)
            RXNativeMethod_push(slotName);
            RXNativeMethod_push(self);
            RXObject_setIsLookingUp(self);
            result = RXObject_respondTo(lookupMethod, RXSymbol_activate_o, RXNil_o, 2);
            RXObject_clearIsLookingUp(self);
        }
    }

    if (result == RXNil_o) {
        // If the delegate of the receiver is not nil,
        // recursively look up in the corresponding object.
        // TODO prevent cycles in the delegate chain
        RXObject_t* delegate = RXObject_delegate(self);
        if (delegate != RXNil_o) {
            result = RXObject_valueOfSlot(delegate, slotName);
        }
    }
    
#ifdef RX_CACHE_ENABLE
    // Create or modify a cached slot in the current object
    RXObject_setSlot(self, slotName, result, true);
#endif

    return result;
}

RXObject_t* RXObject_deleteSlot(RXObject_t* self, RXObject_t* slotName) {
    RXObjectNode_t* node = RXObject_node(self, slotName);
    if (node != NULL) {
        RXObject_t* value = node->value;
        RXObject_coreData(self).slots = eina_rbtree_inline_remove(RXObject_coreData(self).slots, (Eina_Rbtree*)node, EINA_RBTREE_CMP_NODE_CB(RXObject_compareNodes), NULL);
        free(node);
#ifdef RX_CACHE_ENABLE
        RXCache_setDirty(slotName);
#endif
        return value;
    }
    return RXNil_o;
}

RXObject_t* RXObject_respondTo(RXObject_t* self, RXObject_t* messageName, RXObject_t* context, int argumentCount) {
    // Lookup message name starting from the receiver
    RXObject_t* method = RXObject_valueOfSlot(self, messageName);

    // If a method has been found, run it by sending an "activate" message
    if (method != RXNil_o) {
        // method activate(self, ...)
        RXNativeMethod_push(self);
        return RXObject_respondTo(method, RXSymbol_activate_o, context, argumentCount + 1);
    }
    // If no method has been found and the current message is "activate"
    else if (messageName == RXSymbol_activate_o) {
        // If self is a native method, execute it directly
        if (RXObject_isNativeMethod(self)) {
            RXObject_t* receiver = RXNativeMethod_argumentAt(0);
            RXNativeMethod_pop(1);
            RXObject_t* result = RXNativeMethod_activate(self, receiver, context, argumentCount - 1);
            RXNativeMethod_pop(argumentCount - 1);
            return result;
        }
        // If self is not a native, method, return it
        else {
            RXNativeMethod_pop(argumentCount);
            return self;
        }
    }
    // If no method has been found and the current message is not "respondTo"
    else if (messageName != RXSymbol_respondTo_o) {
        RXNativeMethod_push(messageName);
        return RXObject_respondTo(self, RXSymbol_respondTo_o, context, argumentCount + 1);
    }
    // If no method has been found and the current message is not "activate",
    // return nil
    else { 
        RXNativeMethod_pop(argumentCount);
        return RXNil_o;
    }
}

void RXObject_setup(void) {
#ifdef RX_CACHE_ENABLE
    RXCache_data = eina_hash_pointer_new(EINA_FREE_CB(free));
#endif
}

void RXObject_clean(void) {
#ifdef RX_CACHE_ENABLE
    eina_hash_free(RXCache_data);
#endif
}
