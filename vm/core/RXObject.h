
#ifndef __RX_CORE_OBJECT_H__
#define __RX_CORE_OBJECT_H__

#include <stdlib.h>

/*
 * Create an object type with the given name and fields.
 *
 * This example create a struct type with fields x and y.
 * The resulting struct includes the virtual table for object methods.
 *
 * RXObject_defineType(RXPoint_t,
 *      int x;
 *      int y;
 * );
 */
#define RXObject_defineType(name, fields) \
    typedef struct __struct_##name { \
        struct __struct_RXObject_t* __vtable__[0]; \
        fields \
    } name

/*
 * Core object type.
 */
RXObject_defineType(RXObject_t,
    /* Empty */
);

/*
 * Accessor to the VTable of a given object.
 */
#define RXObject_vtable(self) (self)->__vtable__[-1]

/*
 * Allocate memory for a new object with a given amount of payload bytes.
 * This macro allocates memory for the core object data and the payload.
 */
RXObject_t* RXObject_allocateSize(const size_t size);

/*
 * Allocate memory for a new object with type t.
 * This macro allocates memory for the core object data and the payload.
 */
#define RXObject_allocateType(t) \
    RXObject_allocateSize(sizeof(t))

/*
 * Deallocate memory for a previously allocated object.
 */
void RXObject_deallocate(RXObject_t* self);

/*
 * Initialize a newly allocated object.
 * A new object has an empty slot list.
 */
void RXObject_initialize(RXObject_t* self);

/*
 * Prepare the given object to be deleted.
 * This function removes all slots, but does not deallocate the object itself.
 * See RXObject_deallocate.
 */
void RXObject_finalize(RXObject_t* self);

/*
 * Create a new object.
 */
RXObject_t* RXObject_new(void);

/*
 * Create a new object with the given object as delegate.
 */
RXObject_t* RXObject_spawn(RXObject_t* self);

/*
 * Assign a value to a slot of the given object.
 */
void RXObject_setSlot(RXObject_t* self, RXObject_t* slotName, RXObject_t* value);

/*
 * Assign a delegate object to self.
 * If the delegate has a lookup method, it is copied into self.
 */
void RXObject_setDelegate(RXObject_t* self, RXObject_t* delegate);

/*
 * Return the value of a slot in the given object.
 */
RXObject_t* RXObject_valueOfSlot(RXObject_t* self, RXObject_t* name);

/*
 * Delete a slot with the given name from the given object.
 * Return the value of the deleted slot.
 */
RXObject_t* RXObject_deleteSlot(RXObject_t* self, RXObject_t* name);

/*
 * Respond to a message.
 */
RXObject_t* RXObject_respondTo(RXObject_t* self, RXObject_t* messageName, RXObject_t* context, int argumentCount);

#endif
