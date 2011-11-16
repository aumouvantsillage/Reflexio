
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

/*
 * Close a statement in an expression.
 * This method returns the current evaluation context, so that
 * the next message in an expression is sent to the context.
 *
 * In this expression, message a is sent to the current context object.
 * Message d is sent to the object returned by message a.
 *
 *  a(b, c) d(e, f)
 *
 * In this expression message d is sent to the current context object.
 *
 *  a(b, c) ; d(e,f)
 */
static RXNativeMethod_define(RXObject, closeStatement) {
    return context;
}

/*
 * Evaluate an expression in a given context.
 *
 * Argument:
 *  - an object to use as the evaluation context
 *
 * Returns:
 *  - the value of the current expression in the given context
 */
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
RXObject_t* RXSymbol_semicolon_o;

void RXExpression_setup(void) {
    RXSymbol_valueInContext_o = RXSymbol_symbolForCString("valueInContext");
    RXSymbol_semicolon_o = RXSymbol_symbolForCString(";");

    RXExpression_o = RXList_spawn(RXList_o);
    
    RXNativeMethod_attach(RXObject, valueInContext);
    RXNativeMethod_attach(RXExpression, valueInContext);
    
    // Method closeStatement is also bound to symbol ";"    
    RXObject_setSlot(RXObject_o, RXSymbol_semicolon_o,
        RXNativeMethod_attach(RXObject, closeStatement)
    );
}