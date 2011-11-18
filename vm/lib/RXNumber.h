
#ifndef __RX_LIB_NUMBER_H__
#define __RX_LIB_NUMBER_H__

#include <core/RXCore.h>

extern RXObject_t* RXNumber_o;
extern RXObject_t* RXSymbol_equal_o;
extern RXObject_t* RXSymbol_notEqual_o;
extern RXObject_t* RXSymbol_lessThan_o;
extern RXObject_t* RXSymbol_greaterThan_o;
extern RXObject_t* RXSymbol_lessOrEqual_o;
extern RXObject_t* RXSymbol_greaterOrEqual_o;
extern RXObject_t* RXSymbol_add_o;
extern RXObject_t* RXSymbol_subtract_o;
extern RXObject_t* RXSymbol_multiply_o;
extern RXObject_t* RXSymbol_divide_o;
extern RXObject_t* RXSymbol_unaryPlus_o;
extern RXObject_t* RXSymbol_negate_o;

void RXNumber_setup(void);
void RXNumber_clean(void);

#endif
