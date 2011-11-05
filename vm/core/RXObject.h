
#ifndef __RX_CORE_OBJECT_H__
#define __RX_CORE_OBJECT_H__

#include "RXMemory.h"

#include <Eina.h>
#include <stdint.h>

/*
 * Call this macro at the beginning of a struct
 * representing an object type.
 *
 * Example:
 *
 * typedef struct {
 *    // Core object data
 *    RX_OBJECT;
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
 *    RXObject_declareSlots;
 *    Point_t payload;
 * } PointObject_t;
 *
 * See macro RX_OBJECT_TYPE.
 */
#define RXObject_declareSlots Eina_Rbtree* __slots__[0]

/*
 * Create an object type t from a plain C type c.
 */
#define RXObject_defineType(t, c) \
    typedef struct { \
        RXObject_declareSlots; \
        c payload; \
    } t;

/*
 * Accessor to the core data of a given object.
 */
#define RXObject_slots(self) ((self)->__slots__[-1])

/*
 * Allocate memory for a new object with type t.
 * This macro allocates memory for the core object data and the payload.
 */
#define RXCore_allocateObjectOfType(t) RXMemory_allocate(sizeof(t) + sizeof(Eina_Rbtree*))

/*
 * Allocate memory for a new object with a given amount of payload bytes.
 * This macro allocates memory for the core object data and the payload.
 */
#define RXCore_allocateObjectWithSize(s) RXMemory_allocate(s + sizeof(Eina_Rbtree*))

/*
 * Initialize a newly allocated object.
 * Use this macro to initialize the core object data.
 */
#define RXObject_initialize(self) RXObject_slots(self) = NULL

/*
 * Core object type.
 */
typedef struct {
    RXObject_declareSlots;
    // Payload: empty
} RXObject_t;

/*
 * Predefined object: nil
 */
extern RXObject_t* RXNil_o;

/*
 * Create predefined objects used in this module.
 */
void RXObject_setup(void);

/*
 * Allocate and initialize a new object.
 */
RXObject_t* RXObject_new(void);

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

#endif
