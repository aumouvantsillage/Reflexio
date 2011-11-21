
#ifndef __RX_CORE_CACHE_H__
#define __RX_CORE_CACHE_H__

#include "RXObject.h"

void RXCache_setup(void);

void RXCache_addEntry(RXObject_t* object, RXObject_t* slotName, RXObject_t* value);

void RXCache_removeSlotName(RXObject_t* slotName);

RXObject_t* RXCache_valueForEntry(RXObject_t* object, RXObject_t* slotName);

void RXCache_clean(void);

#endif
