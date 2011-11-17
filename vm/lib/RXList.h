
#ifndef __RX_LIB_LIST_H__
#define __RX_LIB_LIST_H__

#include <core/RXObject.h>

extern RXObject_t* RXList_o;

void RXList_setup(void);
void RXList_clean(void);

RXObject_t* RXList_spawn(RXObject_t* self);

// TODO inline
int RXList_count(const RXObject_t* self);

// TODO inline
void RXList_append(RXObject_t* self, RXObject_t* obj);

// TODO inline
RXObject_t* RXList_at(RXObject_t* self, int index);

#endif
