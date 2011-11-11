
#include "RXObject.h"
#include "RXSymbol.h"
#include "RXNativeMethod.h"

#include <inttypes.h>

// Private -------------------------------------------------------------

/*
 * Node type for Red-Black binary trees containing
 * object slots.
 */
typedef struct {
    EINA_RBTREE;
    const RXSymbol_t* key;
    RXObject_t* value;
} RXObjectNode_t;

/*
 * Key comparison callback used during slot lookup.
 */
static int RXObject_compareKeys(const RXObjectNode_t *node, const RXSymbol_t *key, int length, void *data) {
    return (int)((intptr_t)node->key - (intptr_t)key);
}

/*
 * Node comparison callback used diring slot assignment.
 */
static Eina_Rbtree_Direction RXObject_compareNodes(const RXObjectNode_t *left, const RXObjectNode_t *right, RXSymbol_t *slotName) {
    return (RXObject_compareKeys(left, slotName, 0, NULL) < 0)
        ? EINA_RBTREE_LEFT
        : EINA_RBTREE_RIGHT; 
}

static inline Eina_Rbtree* RXObject_node(const RXObject_t* self, const RXSymbol_t* slotName) {
    return eina_rbtree_inline_lookup(RXObject_coreData(self).slots, slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
}

// Public --------------------------------------------------------------

void RXObject_setSlot(RXObject_t* self, const RXSymbol_t* slotName, RXObject_t* value) {
    Eina_Rbtree* node = RXObject_node(self, slotName);
    if (node == NULL) {
        node = malloc(sizeof(RXObjectNode_t));
        RXObject_coreData(self).slots = eina_rbtree_inline_insert(RXObject_coreData(self).slots, node, EINA_RBTREE_CMP_NODE_CB(RXObject_compareNodes), slotName);
        ((RXObjectNode_t*)node)->key = slotName;
    }
    else {
        RXObject_release(((RXObjectNode_t*)node)->value);
    }
    ((RXObjectNode_t*)node)->value = value;
    RXObject_retain(value);
    // TODO remove cache entry if applicable
}

RXObject_t* RXObject_valueOfSlot(const RXObject_t* self, const RXSymbol_t* slotName) {
    Eina_Rbtree* node = RXObject_node(self, slotName);
    return (node == NULL)
        ? RXNil_o
        : ((RXObjectNode_t*)node)->value;
}

void RXObject_deleteSlot(RXObject_t* self, const RXSymbol_t* slotName) {
    Eina_Rbtree* node = RXObject_node(self, slotName);
    if (node != NULL) {
        RXObject_coreData(self).slots = eina_rbtree_inline_remove(RXObject_coreData(self).slots, node, EINA_RBTREE_CMP_NODE_CB(RXObject_compareNodes), NULL);
        RXObject_deleteNode(node, NULL);
    }
    // TODO remove cache entry if applicable
}

void RXObject_deleteNode(Eina_Rbtree* node, void* data) {
    RXObject_release(((RXObjectNode_t*)node)->value);
    free(node);
}

/*
 * Default slot lookup method.
 *
 * If the receiver has a slot with the given name, return the value of that slot.
 * If a lookup method exists in the receiver, send an "activate" message to that method and return the result.
 * If the receiver has a "delegate" slot, recursively look up in the delegate.
 */
static RXObject_t* RXObject_lookup(RXObject_t* self, const RXSymbol_t* slotName) {
    // TODO lookup cache entry
    
    // Prevent infinite recursion in cyclic delegate chains
    if (RXObject_isLookingUp(self)) {
        return RXNil_o;
    }
    
    // If the receiver has a slot with the given name, return the value of that slot.
    Eina_Rbtree* node = eina_rbtree_inline_lookup(RXObject_coreData(self).slots, slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    if (node != NULL) {
        return ((RXObjectNode_t*)node)->value;
    }
    
    // If a lookup method exists in the receiver, send an "activate" message to that method and return the result.
    node = eina_rbtree_inline_lookup(RXObject_coreData(self).slots, RXSymbol_lookup_o, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    if (node != NULL) {
        RXNativeMethod_push((RXObject_t*)slotName);
        RXNativeMethod_push(self);
        RXObject_t* result = RXObject_respondTo(((RXObjectNode_t*)node)->value, RXSymbol_activate_o, 2);
        RXNativeMethod_pop(2);
        return result;
    }

    // If a delegate slot exists in the receiver, recursively look up in the corresponding object
    node = eina_rbtree_inline_lookup(RXObject_coreData(self).slots, RXSymbol_delegate_o, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    if (node != NULL) {
        RXObject_setIsLookingUp(self);
        RXObject_t* result =  RXObject_lookup(((RXObjectNode_t*)node)->value, slotName);
        RXObject_clearIsLookingUp(self);
        return result;
    }
        
    return RXNil_o;
}

RXObject_t* RXObject_respondTo(RXObject_t* self, const RXSymbol_t* messageName, int argumentCount) {
    RXObject_t* method = RXObject_lookup(self, messageName);

    // If a method has been found, run it by sending an "activate" message
    if (method != RXNil_o) {
        RXNativeMethod_push(self);
        RXObject_t* result = RXObject_respondTo(method, RXSymbol_activate_o, argumentCount + 1);
        RXNativeMethod_pop(1);
        return result;        
    }
    // If no method has been found and the current message is "activate"
    else if (messageName == RXSymbol_activate_o) {
        // If self is a native method, execute it directly
        // else return the receiver
        if (RXObject_isNativeMethod(self)) {
            return RXNativeMethod_activate((RXNativeMethod_t*)self, RXNativeMethod_argumentAt(0), argumentCount);
        }
        else {
            return self;        
        }
    }
    // If no method has been found and the current message is not activate,
    // return nil
    else { 
        return RXNil_o;
    }
}
