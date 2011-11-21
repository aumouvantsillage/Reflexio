
#include "RXCore.h"
#include <inttypes.h>

// Private -------------------------------------------------------------

inline static bool RXObject_isLookingUp(const RXObject_t* self) {
    return (RXObject_coreData(self).flags & RXObject_flagIsLookingUp) != 0;
}

inline static void RXObject_setIsLookingUp(RXObject_t* self) { 
    RXObject_coreData(self).flags |= RXObject_flagIsLookingUp;
}

inline static void RXObject_clearIsLookingUp(RXObject_t* self) {
    RXObject_coreData(self).flags &= ~RXObject_flagIsLookingUp;
}

/*
 * Node type for Red-Black binary trees containing
 * object slots.
 */
typedef struct {
    EINA_RBTREE;
    RXObject_t* key;
    RXObject_t* value;
} RXObjectNode_t;

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

static inline RXObjectNode_t* RXObject_node(const RXObject_t* self, const RXObject_t* slotName) {
    return (RXObjectNode_t*)eina_rbtree_inline_lookup(RXObject_coreData(self).slots, slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
}

// Public --------------------------------------------------------------

void RXObject_setSlot(RXObject_t* self, RXObject_t* slotName, RXObject_t* value) {
    RXObjectNode_t* node = RXObject_node(self, slotName);
    if (node == NULL) {
        node = malloc(sizeof(RXObjectNode_t));
        RXObject_coreData(self).slots = eina_rbtree_inline_insert(RXObject_coreData(self).slots, (Eina_Rbtree*)node, EINA_RBTREE_CMP_NODE_CB(RXObject_compareNodes), slotName);
        node->key = slotName;
    }
    else {
        // TODO remove cache entry for old value
    }
    node->value = value;
}

void RXObject_setDelegate(RXObject_t* self, RXObject_t* delegate) {
    RXObject_setSlot(self, RXSymbol_delegate_o, delegate);
    RXObjectNode_t* node = RXObject_node(delegate, RXSymbol_lookup_o);
    if (node != NULL) {
        RXObject_setSlot(self, RXSymbol_lookup_o, node->value);
    }
}

/*
 * Default slot lookup method.
 *
 * If the receiver has a slot with the given name, return the value of that slot.
 * If a lookup method exists in the receiver, send an "activate" message to that method and return the result.
 * If the receiver has a "delegate" slot, recursively look up in the delegate.
 */
RXObject_t* RXObject_valueOfSlot(RXObject_t* self, RXObject_t* slotName) {
    // TODO lookup cache entry
    
    // Prevent infinite recursion in cyclic delegate chains
    // NULL indicates failure
    if (RXObject_isLookingUp(self)) {
        return RXNil_o;
    }
    
    RXObject_setIsLookingUp(self);
    RXObject_t* result = RXNil_o;
        
    // Look up in the own slots of the current object
    RXObjectNode_t* node = RXObject_node(self, slotName);
    if (node != NULL) {
        result = node->value;
    }

    if (result == RXNil_o) {
        // If a lookup method exists in the receiver, send an "activate" message to that method
        node = RXObject_node(self, RXSymbol_lookup_o);
        if (node != NULL) {
            // lookupMethod activate(self, slotName)
            RXNativeMethod_push(slotName);
            RXNativeMethod_push(self);
            result = RXObject_respondTo(node->value, RXSymbol_activate_o, RXObject_o, 2);
        }
    }

    if (result == RXNil_o) {
        // If a delegate slot exists in the receiver, recursively look up in the corresponding object
        node = RXObject_node(self, RXSymbol_delegate_o);
        if (node != NULL) {
            result =  RXObject_valueOfSlot(node->value, slotName);
        }
    }
    
    RXObject_clearIsLookingUp(self);
    return result;
}

RXObject_t* RXObject_deleteSlot(RXObject_t* self, RXObject_t* slotName) {
    RXObjectNode_t* node = RXObject_node(self, slotName);
    if (node != NULL) {
        RXObject_t* value = node->value;
        RXObject_coreData(self).slots = eina_rbtree_inline_remove(RXObject_coreData(self).slots, (Eina_Rbtree*)node, EINA_RBTREE_CMP_NODE_CB(RXObject_compareNodes), NULL);
        free(node);
        // TODO remove cache entry for old value
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
