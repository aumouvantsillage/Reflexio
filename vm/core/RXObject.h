
#ifndef __RX_CORE_OBJECT_H__
#define __RX_CORE_OBJECT_H__

#include <stdint.h>
#include <Eina.h>

/*
 * Common data for all objects.
 */
typedef struct {
    Eina_Rbtree* slots;
    uint32_t flags;
} RXObjectCoreData_t;

enum {
    RXObject_flagIsLookingUp = 1,
    RXObject_flagIsNativeMethod = 2
};

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
 * Core object type.
 */
typedef struct {
    RXObject_declaration;
    // Payload: empty
}  RXObject_t;

/*
 * Assign a value to a slot of the given object.
 */
void RXObject_setSlot(RXObject_t* self, RXObject_t* slotName, RXObject_t* value);

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
