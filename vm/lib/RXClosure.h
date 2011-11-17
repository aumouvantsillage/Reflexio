
#ifndef __RX_LIB_CLOSURE_H__
#define __RX_LIB_CLOSURE_H__

#include <core/RXCore.h>

extern RXObject_t* RXClosure_o;
extern RXObject_t* RXSymbol_Closure_o;
extern RXObject_t* RXSymbol_self_o;
extern RXObject_t* RXSymbol_locals_o;
extern RXObject_t* RXSymbol_context_o;
extern RXObject_t* RXSymbol_arguments_o;
extern RXObject_t* RXSymbol_parameters_o;
extern RXObject_t* RXSymbol_body_o;
extern RXObject_t* RXSymbol_isMethod_o;
extern RXObject_t* RXSymbol_method_o;
extern RXObject_t* RXSymbol_block_o;

void RXClosure_setup(void);

#endif
