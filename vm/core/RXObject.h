
#ifndef __RX_CORE_OBJECT_H__
#define __RX_CORE_OBJECT_H__

#include <stdint.h>
#include <Eina.h>

/*
 * Common data for all objects.
 */
typedef struct {
    Eina_Rbtree* slots;
    struct __struct_RXObject_t* delegate;
    uint32_t flags;
} RXObjectCoreData_t;

enum {
    RXObject_flagIsLookingUp = 1,
    RXObject_flagIsNativeMethod = 2
};

/*
 * Create an object type.
 *
 * Example: a point type with native attributes x and y.
 *
 * RXObject_defineType(Point_t,
 *      int x;
 *      int y;
 * );
 */
#define RXObject_defineType(name, declarations) \
    typedef struct __struct_##name { \
        RXObjectCoreData_t __coreData__[0]; \
        declarations \
    } name

/*
 * Core object type.
 */
RXObject_defineType(RXObject_t,
    /* Empty */
);

/*
 * Assign a value to a slot of the given object.
 */
void RXObject_setSlot(RXObject_t* self, RXObject_t* slotName, RXObject_t* value);

/*
 * Assign a delegate object to self.
 * If the delegate has a lookup method, it is copied into self.
 */
inline static void RXObject_setDelegate(RXObject_t* self, RXObject_t* delegate);

/*
 * Return the value of a slot in the given object.
 */
RXObject_t* RXObject_valueOfSlot(RXObject_t* self, RXObject_t* slotName);

/*
 * Delete a slot with the given name from the given object.
 * Return the value of the deleted slot.
 */
RXObject_t* RXObject_deleteSlot(RXObject_t* self, RXObject_t* slotName);

/*
 * Respond to a message.
 */
RXObject_t* RXObject_respondTo(RXObject_t* self, RXObject_t* messageName, RXObject_t* context, int argumentCount);

#endif
