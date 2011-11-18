
#ifndef __RX_LIB_OBJECT_H__
#define __RX_LIB_OBJECT_H__

#include <core/RXCore.h>

extern RXObject_t* RXLobby_o;
extern RXObject_t* RXSymbol_Lobby_o;
extern RXObject_t* RXSymbol_Object_o;
extern RXObject_t* RXSymbol_spawn_o;
extern RXObject_t* RXSymbol_setSlot_o;
extern RXObject_t* RXSymbol_valueOfSlot_o;
extern RXObject_t* RXSymbol_deleteSlot_o;
extern RXObject_t* RXSymbol_print_o;
extern RXObject_t* RXSymbol_println_o;
extern RXObject_t* RXSymbol_asString_o;
extern RXObject_t* RXSymbol_asSource_o;
extern RXObject_t* RXSymbol_nil_o;

void RXObject_libSetup(void);

#endif
