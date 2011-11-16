
#include "RXLib.h"

// Methods -------------------------------------------------------------

/*
 * Return the value of the receiver in the given context.
 * This method provides a default behavior for expression evaluation.
 * See RXExpression.
 *
 * Arguments:
 *  - the evaluation context
 *
 * Returns:
 *  - self
 */
static RXNativeMethod_define(RXObject, valueInContext) {
    return self;
}

static RXNativeMethod_define(RXExpression, valueInContext) {
    RXObject_t* evalContext = RXExpression_valueOfArgumentAt(0, context);
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

// Public --------------------------------------------------------------

RXObject_t* RXExpression_o;
RXObject_t* RXSymbol_valueInContext_o;

void RXExpression_setup(void) {
    RXSymbol_valueInContext_o = RXSymbol_symbolForCString("valueInContext");

    RXExpression_o = RXList_spawn(RXList_o);
    
    RXNativeMethod_attach(RXObject, valueInContext);
    RXNativeMethod_attach(RXExpression, valueInContext);
}
