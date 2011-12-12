
#ifndef __RX_CORE_H__
#define __RX_CORE_H__

#define RX_INLINE __attribute__((always_inline)) inline static

#include "RXObject.h"

extern RXObject_t* RXLobby_o;
extern RXObject_t* RXProtoObject_o;
extern RXObject_t* RXSymbol_Lobby_o;
extern RXObject_t* RXSymbol_ProtoObject_o;
extern RXObject_t* RXSymbol_Object_o;
extern RXObject_t* RXSymbol_nil_o;
extern RXObject_t* RXSymbol_o;
extern RXObject_t* RXSymbol_activate_o;
extern RXObject_t* RXSymbol_lookup_o;
extern RXObject_t* RXSymbol_respondTo_o;
extern RXObject_t* RXObject_o;
extern RXObject_t* RXNil_o;
extern RXObject_t* RXNativeMethod_o;
extern RXObject_t* RXSymbol_NativeMethod_o;

#include "RXObject_inline.h"
#include "RXSymbol.h"
#include "RXNativeMethod.h"
#include "RXNativeMethod_inline.h"

void RXCore_setup(void);
void RXCore_clean(void);

#endif
