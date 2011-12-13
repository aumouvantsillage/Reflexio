
#ifndef __RX_CORE_CACHE_H__
#define __RX_CORE_CACHE_H__

#ifdef RX_CACHE_ENABLE

#include "RXObject.h"

typedef unsigned RXCacheVersion_t;

void RXCache_setup(void);

RXCacheVersion_t RXCache_addSlotName(RXObject_t* slotName);

void RXCache_setDirty(RXObject_t* slotName);

RXCacheVersion_t RXCache_version(RXObject_t* slotName);

void RXCache_removeSlotName(RXObject_t* slotName);

void RXCache_clean(void);

#endif

#endif
