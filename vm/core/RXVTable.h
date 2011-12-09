
#ifndef __RX_CORE_VTABLE_H__
#define __RX_CORE_VTABLE_H__

#include "RXObject.h"

/*
 * Create a new VTable object.
 */
RXObject_t* RXVTable_new(void);

/*
 * Delete a VTable object.
 */
void RXVTable_delete(RXObject_t*);

/*
 * Add or replace a method in the current VTable.
 *
 * Parameters:
 *  - self: the current VTable
 *  - name: the name of the method to set
 *  - method: the method object to set
 */
void RXVTable_setMethodForName(RXObject_t* self, RXObject_t* name, RXObject_t* method);

/*
 * Return the method with the given name in the scope of the current VTable.
 * If the method does not exist in the current VTable, the "lookup" message is sent
 * to the current VTable.
 *
 * Parameters:
 *  - self: the current VTable
 *  - name: the name of the method to look up
 *
 * Returns:
 *  - a method object if found
 *  - RXVTable_notFound_o if no method has been found
 */
RXObject_t* RXVTable_methodForName(const RXObject_t* self, const RXObject_t* name);

/*
 * Remove and return the method with the given name in the current VTable.
 *
 * Parameters:
 *  - self: the current VTable
 *  - name: the name of the method to remove
 *
 * Returns:
 *  - a method object if found
 *  - RXMethod_notFound_o if no method has been found
 */
RXObject_t* RXVTable_removeMethodForName(RXObject_t* self, const RXObject_t* name);

#endif
