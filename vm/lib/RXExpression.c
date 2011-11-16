
#include "RXLib.h"

static RXNativeMethod_define(RXExpression, valueInContext) {
    RXObject_t* evalContext = RXObject_valueOfArgumentAt(0, context);
    RXObject_t* receiver = evalContext;
    
    Eina_List* iter;
    RXObject_t* msg;
    EINA_LIST_REVERSE_FOREACH(*(Eina_List**)self, iter, msg) {
        RXNativeMethod_push(evalContext);
        RXNativeMethod_push(receiver);
        receiver = RXObject_respondTo(msg, RXSymbol_valueOnObjectInContext_o, RXNil_o, 2);
    }
    
    return receiver;
}

RXObject_t* RXExpression_o;

void RXExpression_setup(void) {
    RXExpression_o = RXList_spawn(RXList_o);
    
    RXNativeMethod_attach(RXExpression, valueInContext);
}
