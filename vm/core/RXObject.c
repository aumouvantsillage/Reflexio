
#include "RXObject.h"

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
    free(node);
    // TODO delete contained object based on reference counting
}

void RXObject_setSlot(RXObject_t* self, const RXSymbol_t* slotName, RXObject_t* value) {
    Eina_Rbtree* node = eina_rbtree_inline_lookup(RXObject_slots(self), slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    if (node == NULL) {
        node = malloc(sizeof(RXObjectNode_t));
        RXObject_setSlots(self, eina_rbtree_inline_insert(RXObject_slots(self), node, EINA_RBTREE_CMP_NODE_CB(RXObject_compareNodes), slotName));
    }
    ((RXObjectNode_t*)node)->key = slotName;
    ((RXObjectNode_t*)node)->value = value;
    // TODO remove cache entry if applicable
}

RXObject_t* RXObject_valueOfSlot(const RXObject_t* self, const RXSymbol_t* slotName) {
    Eina_Rbtree* node = eina_rbtree_inline_lookup(RXObject_slots(self), slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    return (node == NULL)
        ? RXNil_o
        : ((RXObjectNode_t*)node)->value;
}

