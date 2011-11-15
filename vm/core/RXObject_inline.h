
#ifndef __RX_CORE_OBJECT_INLINE_H__
#define __RX_CORE_OBJECT_INLINE_H__

#include "RXCore.h"
#include <stdbool.h>
#include <assert.h>

/*
 * Accessor to the core data of a given object.
 */
#define RXObject_coreData(self) (self)->__coreData__[-1]

/*
 * Allocate memory for a new object with a given amount of payload bytes.
 * This macro allocates memory for the core object data and the payload.
 */
inline static RXObject_t* RXObject_allocateSize(const size_t size) {
    return (RXObject_t*)((char*)malloc(size + sizeof(RXObjectCoreData_t)) + sizeof(RXObjectCoreData_t));
}

/*
 * Allocate memory for a new object with type t.
 * This macro allocates memory for the core object data and the payload.
 */
#define RXObject_allocateType(t) RXObject_allocateSize(sizeof(t))

/*
 * Deallocate memory for a previously allocated object.
 */
inline static void RXCore_deallocateObject(RXObject_t* self) {
    free((char*)self - sizeof(RXObjectCoreData_t));
}

inline static bool RXObject_isReachable(const RXObject_t* self) {
    return (RXObject_coreData(self).flags & RXObject_flagIsReachable) != 0;
}

inline static void RXObject_setIsReachable(RXObject_t* self) { 
    RXObject_coreData(self).flags |= RXObject_flagIsReachable;
}

inline static void RXObject_clearIsReachable(RXObject_t* self) {
    RXObject_coreData(self).flags &= ~RXObject_flagIsReachable;
}

/*
 * The pool of objects that are possibly unreachable.
 */
extern Eina_Hash* RXObject_pool;

/*
 * Mark the given object as reachable.
 */
inline static void RXObject_mark(RXObject_t* obj) {
    RXObject_setIsReachable(obj);
    eina_hash_del_by_key(RXObject_pool, obj);
}

/*
 * Mark the given object as possibly unreachable.
 */
inline static void RXObject_unmark(RXObject_t* obj) {
    RXObject_clearIsReachable(obj);
    eina_hash_add(RXObject_pool, obj, obj);
}

/*
 * Mark the given object as reachable if self is reachable.
 */
inline static void RXObject_chainMark(const RXObject_t* self, RXObject_t* obj) {
    if (RXObject_isReachable(self) && !RXObject_isReachable(obj)) {
        RXObject_mark(obj);
    }
}

/*
 * Initialize a newly allocated object.
 * Use this macro to initialize the core object data.
 */
inline static void RXObject_initialize(RXObject_t* self) {
    RXObject_coreData(self).slots = NULL;
    RXObject_coreData(self).flags = 0;
    eina_hash_add(RXObject_pool, self, self); // FIXME direct add?
}

/*
 * Allocate and initialize a new object.
 * This function does not set the delegate slot.
 * See RXObject_spawn.
 */
inline static RXObject_t* RXObject_new(void) {
    RXObject_t* self = RXObject_allocateType(RXObject_t);
    RXObject_initialize(self);
    return self;
}

/*
 * Allocate and initialize a new object.
 * Set the delegate slot of the new object to self.
 */
inline static RXObject_t* RXObject_spawn(RXObject_t* self) {
    RXObject_t* result = RXObject_new();
    RXObject_setSlot(result, RXSymbol_delegate_o, self);
    return result;
}

/*
 * Prepare the given object to be deleted.
 * This function removes all slots, but does not deallocate the object itself.
 * See RXCore_deallocateObject.
 */
inline static void RXObject_finalize(RXObject_t* self) {
    eina_rbtree_delete(RXObject_coreData(self).slots, EINA_RBTREE_FREE_CB(free), NULL);
}

#endif
