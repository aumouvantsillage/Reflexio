
#ifndef __RX_LIB_MESSAGE_H__
#define __RX_LIB_MESSAGE_H__

#include <core/RXCore.h>

extern RXObject_t* RXMessage_o;
extern RXObject_t* RXSymbol_Message_o;
extern RXObject_t* RXSymbol_name_o;
extern RXObject_t* RXSymbol_valueOnObjectInContext_o;

void RXMessage_setup(void);

// TODO replace List with Array
#define RXMessage_spawn RXList_spawn

#endif
