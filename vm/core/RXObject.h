
#ifndef __RX_CORE_OBJECT_H__
#define __RX_CORE_OBJECT_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <Eina.h>

typedef struct {
    Eina_Rbtree* slots;
    uint32_t meta;
} RXObjectCoreData_t;

/*
 * Call this macro at the beginning of a struct
 * representing an object type.
 *
 * Example:
 *
 * typedef struct {
 *    // Core object data
 *    RXObject_declaration;
 *    // Payload
 *    int x;
 *    int y;
 * } PointObject_t;
 *
 * Object data is transparent for interoperability with plain C functions.
 * If a C function expects data of this type:
 *
 * typedef struct {
 *    int x;
 *    int y;
 * } Point_t;
 *
 * You can define a compatible object type like this:
 *
 * typedef struct {
 *    RXObject_declaration;
 *    Point_t payload;
 * } PointObject_t;
 *
 * See macro RXObject_defineType.
 */
#define RXObject_declaration RXObjectCoreData_t __coreData__[0]

/*
 * Create an object type t from a plain C type c.
 */
#define RXObject_defineType(t, c) \
    typedef struct { \
        RXObject_declaration; \
        c payload; \
    } t

/*
 * Accessor to the core data of a given object.
 */
#define RXObject_coreData(self) (self)->__coreData__[-1]

#define RXObject_slots(self) RXObject_coreData(self).slots

#define RXObject_flagIsLookingUp (1<<31)
#define RXObject_flagIsNativeMethod (1<<30)
#define RXObject_flags (RXObject_flagIsLookingUp | RXObject_flagIsNativeMethod)

#define RXObject_isLookingUp(self) (RXObject_coreData(self).meta & RXObject_flagIsLookingUp)

#define RXObject_setIsLookingUp(self) RXObject_coreData(self).meta |= RXObject_flagIsLookingUp

#define RXObject_clearIsLookingUp(self) RXObject_coreData(self).meta &= ~RXObject_flagIsLookingUp

#define RXObject_isNativeMethod(self) (RXObject_coreData(self).meta & RXObject_flagIsNativeMethod)

#define RXObject_setIsNativeMethod(self) RXObject_coreData(self).meta |= RXObject_flagIsNativeMethod

#define RXObject_clearIsNativeMethod(self) RXObject_coreData(self).meta &= ~RXObject_flagIsNativeMethod

#define RXObject_retainCount(self) (RXObject_coreData(self).meta & ~RXObject_flags)

#define RXObject_retain(self) RXObject_coreData(self).meta++

#define RXObject_release(self) \
    RXObject_coreData(self).meta--; \
    if (!RXObject_retainCount(self)) { \
        /* TODO send "delete" message to self */ \
    }

#define RXObject_sizeOfCoreData sizeof(RXObjectCoreData_t)

/*
 * Allocate memory for a new object with type t.
 * This macro allocates memory for the core object data and the payload.
 */
#define RXCore_allocateObjectOfType(t) (t*)((char*)malloc(sizeof(t) + RXObject_sizeOfCoreData) + RXObject_sizeOfCoreData)

/*
 * Allocate memory for a new object with a given amount of payload bytes.
 * This macro allocates memory for the core object data and the payload.
 */
#define RXCore_allocateObjectWithSize(t, s) (t*)((char*)malloc((s) + RXObject_sizeOfCoreData) + RXObject_sizeOfCoreData)

/*
 * Deallocate memory for a previously allocated object.
 */
#define RXCore_deallocateObject(p) free((char*)(p) - RXObject_sizeOfCoreData)

/*
 * Initialize a newly allocated object.
 * Use this macro to initialize the core object data.
 */
#define RXObject_initialize(self) \
    RXObject_slots(self) = NULL; \
    RXObject_coreData(self).meta = 0;

/*
 * Prepare the given object to be deleted
 */
#define RXObject_finalize(self) eina_rbtree_delete(RXObject_slots(self), RXObject_deleteSlot, NULL)

/*
 * Core object type.
 */
typedef struct {
    RXObject_declaration;
    // Payload: empty
}  RXObject_t;

/*
 * Predefined object: nil
 */
extern RXObject_t* RXNil_o;

/*
 * Create predefined objects used in this module.
 */
void RXObject_setup(void);

/*
 * Delete all objects created by this module.
 */
void RXObject_clean(void);

/*
 * Allocate and initialize a new object.
 */
RXObject_t* RXObject_new(void);

/*
 * Deallocate and uninitialize an object.
 */
void RXObject_delete(RXObject_t* self);

/*
 * Delete a slot.
 */
void RXObject_deleteSlot(Eina_Rbtree* node, void* data);

/*
 * Symbol type, defined in RXSymbol.h
 */
typedef struct RXSymbol_s RXSymbol_t;

/*
 * Assign a value to a slot of the given object.
 */
void RXObject_setSlot(RXObject_t* self, const RXSymbol_t* slotName, RXObject_t* value);

/*
 * Return the value of a slot in the given object.
 */
RXObject_t* RXObject_valueOfSlot(const RXObject_t* self, const RXSymbol_t* slotName);

/*
 * Respond to a message.
 */
RXObject_t* RXObject_respondTo(RXObject_t* self, const RXSymbol_t* messageName, RXObject_t* context, const int argumentCount, RXObject_t* arguments);

#endif
