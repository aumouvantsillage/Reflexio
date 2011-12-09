
#ifndef __RX_CORE_H__
#define __RX_CORE_H__

#include "RXObject.h"
#include "RXSlot.h"
#include "RXVTable.h"

extern RXObject_t* RXVTable_o;
extern RXObject_t* RXObject_o;
extern RXObject_t* RXSlot_o;
extern RXObject_t* RXSymbol_o;
extern RXObject_t* RXNil_o;
extern RXObject_t* RXNativeMethod_o;
extern RXObject_t* RXNativeMethod_methodNotFound_o;
extern RXObject_t* RXSymbol_activate_o;
extern RXObject_t* RXSymbol_delegate_o;
extern RXObject_t* RXSymbol_lookup_o;
extern RXObject_t* RXSymbol_respondTo_o;

#include "RXSymbol.h"
#include "RXNativeMethod.h"

void RXCore_setup(void);
void RXCore_clean(void);

#endif
