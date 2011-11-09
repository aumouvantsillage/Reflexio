
#include "RXObject.h"
#include "RXSymbol.h"
#include "RXNativeMethod.h"

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

// Public --------------------------------------------------------------

RXObject_t* RXNil_o;

void RXObject_setup(void) {
    RXNil_o = RXObject_new();
}

void RXObject_clean(void) {
    RXObject_delete(RXNil_o); 
}

RXObject_t* RXObject_new(void) {
    RXObject_t* obj = RXCore_allocateObjectOfType(RXObject_t);
    RXObject_initialize(obj);
    return obj;
}

void RXObject_delete(RXObject_t* self) {
    RXObject_finalize(self);
    RXCore_deallocateObject(self);
}

void RXObject_deleteSlot(Eina_Rbtree* node, void* data) {
    RXObject_release(((RXObjectNode_t*)node)->value);
    free(node);
}

void RXObject_setSlot(RXObject_t* self, const RXSymbol_t* slotName, RXObject_t* value) {
    Eina_Rbtree* node = eina_rbtree_inline_lookup(RXObject_slots(self), slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    if (node == NULL) {
        node = malloc(sizeof(RXObjectNode_t));
        RXObject_slots(self) = eina_rbtree_inline_insert(RXObject_slots(self), node, EINA_RBTREE_CMP_NODE_CB(RXObject_compareNodes), slotName);
    }
    else {
        RXObject_release(((RXObjectNode_t*)node)->value);
    }
    ((RXObjectNode_t*)node)->key = slotName;
    ((RXObjectNode_t*)node)->value = value;
    RXObject_retain(value);
    // TODO remove cache entry if applicable
}

RXObject_t* RXObject_valueOfSlot(const RXObject_t* self, const RXSymbol_t* slotName) {
    Eina_Rbtree* node = eina_rbtree_inline_lookup(RXObject_slots(self), slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    return (node == NULL)
        ? RXNil_o
        : ((RXObjectNode_t*)node)->value;
}

/*
 * Default slot lookup method.
 *
 * If the receiver has a slot with the given name, return the value of that slot.
 * If a lookup method exists in the receiver, send an "activate" message to that method and return the result.
 * If the receiver has a "delegate" slot, recursively look up in the delegate.
 */
static RXObject_t* RXObject_lookup(RXObject_t* self, const RXSymbol_t* slotName) {
    // Prevent infinite recursion in cyclic delegate chains
    if (RXObject_isLookingUp(self))
        return RXNil_o;
        
    // If the receiver has a slot with the given name, return the value of that slot.
    Eina_Rbtree* node = eina_rbtree_inline_lookup(RXObject_slots(self), slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    if (node != NULL) {
        return ((RXObjectNode_t*)node)->value;
    }
    
    // If a lookup method exists in the receiver, send an "activate" message to that method and return the result.
    node = eina_rbtree_inline_lookup(RXObject_slots(self), RXSymbol_lookup_o, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    if (node != NULL) {
        RXObject_t* arguments[2] = {self, (RXObject_t*)slotName};
        return RXObject_respondTo(((RXObjectNode_t*)node)->value, RXSymbol_activate_o, RXNil_o, 2, arguments);
    }

    return RXNil_o;
}

RXObject_t* RXObject_respondTo(RXObject_t* self, const RXSymbol_t* messageName, RXObject_t* context, const int argumentCount, RXObject_t* arguments) {
    RXObject_t* method = RXObject_lookup(self, messageName);
    if (method == RXNil_o) {
        return self;
    }
    else if (RXObject_isNativeMethod(method)) {
        return RXNativeMethod_activate((RXNativeMethod_t*)method, self, context, argumentCount, arguments);
    }
    else {
        return RXObject_respondTo(method, RXSymbol_activate_o, RXNil_o, 2, arguments);
    }
}
