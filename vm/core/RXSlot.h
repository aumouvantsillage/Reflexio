
#ifndef __RX_CORE_SLOT_H__
#define __RX_CORE_SLOT_H__

#include "RXObject.h"

RXObject_t* RXSlot_new(RXObject_t* value);

RXObject_t* RXSlot_spawn(RXObject_t* self, RXObject_t* value);

#endif
