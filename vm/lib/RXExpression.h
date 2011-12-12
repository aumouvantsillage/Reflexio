
#ifndef __RX_LIB_EXPRESSION_H__
#define __RX_LIB_EXPRESSION_H__

extern RXObject_t* RXExpression_o;
extern RXObject_t* RXSymbol_Expression_o;
extern RXObject_t* RXSymbol_valueInContext_o;
extern RXObject_t* RXSymbol_semicolon_o;
extern RXObject_t* RXSymbol_fromFile_o;
extern RXObject_t* RXSymbol_fromString_o;

void RXExpression_setup();

RXObject_t* RXExpression_valueOfArgumentAt(int index, RXObject_t* context);

#endif
