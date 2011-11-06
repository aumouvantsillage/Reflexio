
#include "RXObject.h"

#include <Eina.h>

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

/*
 * The core data field is decomposed as follows
 * - bits 0 and 1: flags
 * - bits 2 to MSB: pointer to the slots container
 *
 * On 32-bit and 64 bit platforms, we assume that bits 0 and 1 of a pointer are always 0.
 * We use these bits to store flags.
 */
#define RXObject_coreFlagsMask 3
#define RXObject_coreSlotsMask (~RXObject_coreFlagsMask)

#define RXObject_slots(self) (Eina_Rbtree*)(RXObject_coreData(self) & RXObject_coreSlotsMask)
#define RXObject_flags(self) RXObject_coreData(self) & RXObject_coreFlagsMask
#define RXObject_setSlots(self, expr) RXObject_coreData(self) = RXObject_flags(self) | (intptr_t)(expr) & RXObject_coreSlotsMask

// Public --------------------------------------------------------------

RXObject_t* RXNil_o;

void RXObject_setup(void) {
    RXNil_o = RXObject_new();
}

RXObject_t* RXObject_new(void) {
    RXObject_t* obj = RXCore_allocateObjectOfType(RXObject_t);
    RXObject_initialize(obj);
    return obj;
}

void RXObject_setSlot(RXObject_t* self, const RXSymbol_t* slotName, RXObject_t* value) {
    Eina_Rbtree* node = eina_rbtree_inline_lookup(RXObject_slots(self), slotName, 0, EINA_RBTREE_CMP_KEY_CB(RXObject_compareKeys), NULL);
    if (node == NULL) {
        node = RXMemory_allocate(sizeof(RXObjectNode_t));
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

