
#include "RXLib.h"
#include <parser/RXParser.h>

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
RXNativeMethod_define(RXProtoObject, valueInContext) {
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
RXNativeMethod_define(RXProtoObject, closeStatement) {
    return context;
}

/*
 * Evaluate an expression in a given context.
 *
 * Argument:
 *  - an object to use as the evaluation context
 *    if missing, the expression is evaluated in the context of nil
 *
 * Returns:
 *  - the value of the current expression in the given context
 */
RXNativeMethod_define(RXExpression, valueInContext) {
    RXObject_t* evalContext = argumentCount > 0
        ? RXExpression_valueOfArgumentAt(0, context)
        : RXNil_o;
        
    RXObject_t* receiver = evalContext;
    
    Eina_List* iter;
    RXObject_t* msg;
    EINA_LIST_FOREACH(*(Eina_List**)self, iter, msg) {
        RXNativeMethod_push(evalContext);
        RXNativeMethod_push(receiver);
        receiver = RXObject_respondTo(msg, RXSymbol_valueOnObjectInContext_o, RXNil_o, 2);
    }
    
    // FIXME return nil if expression is empty?
    return receiver;
}

RXNativeMethod_define(RXExpression, asString) {
    int count = RXList_count(self);
    RXObject_t* msgSrc[count];
    int len = 0;
    int index = 0;
    Eina_List* iter;
    RXObject_t* msg;
    EINA_LIST_FOREACH(*(Eina_List**)self, iter, msg) {
        RXObject_t* src = RXObject_respondTo(msg, RXSymbol_asSource_o, RXNil_o, 0);
        len += strlen((char*)src) + 1;
        msgSrc[index++] = src;
    }
    char result[len];
    result[0] = 0;
    for (index = 0; index < count; index ++) {
        if (index > 0) {
            strcat(result, " ");
        }
        strcat(result, (char*)(msgSrc[index]));
    }
    return RXSymbol_symbolForCString(result);
}

/*
 * Create a new expression from the given file.
 *
 * Arguments:
 *  - a file object open for reading
 *
 * Returns:
 *  - a new expression
 */
RXNativeMethod_define(RXExpression, fromFile) {
    if (argumentCount > 0) {
        // FIXME check that argument is a string
        RXObject_t* fileName = RXExpression_valueOfArgumentAt(0, context);
        return RXParser_expressionFromCFile((char*)fileName);
    }
    else {
        // FIXME raise exception
        return RXNil_o;
    }
}

/*
 * Create a new expression from the given string.
 *
 * Arguments:
 *  - a string containing an expression
 *
 * Returns:
 *  - a new expression
 */
RXNativeMethod_define(RXExpression, fromString) {
    if (argumentCount > 0) {
        // FIXME check that argument is a string
        RXObject_t* str = RXExpression_valueOfArgumentAt(0, context);
        return RXParser_expressionFromCString((char*)str);
    }
    else {
        // FIXME raise exception
        return RXNil_o;
    }
}

// Public --------------------------------------------------------------

RXObject_t* RXExpression_o;
RXObject_t* RXSymbol_Expression_o;
RXObject_t* RXSymbol_valueInContext_o;
RXObject_t* RXSymbol_semicolon_o;
RXObject_t* RXSymbol_fromFile_o;
RXObject_t* RXSymbol_fromString_o;

void RXExpression_setup(void) {
    RXSymbol_valueInContext_o = RXSymbol_symbolForCString("valueInContext");
    RXSymbol_semicolon_o = RXSymbol_symbolForCString(";");
    RXSymbol_fromFile_o = RXSymbol_symbolForCString("fromFile");
    RXSymbol_fromString_o = RXSymbol_symbolForCString("fromString");

    RXExpression_o = RXList_spawn(RXList_o, NULL);
    
    RXNativeMethod_attach(RXProtoObject, valueInContext);
    RXNativeMethod_attach(RXExpression, valueInContext);
    RXNativeMethod_attach(RXExpression, asString);
    RXNativeMethod_attach(RXExpression, fromFile);
    RXNativeMethod_attach(RXExpression, fromString);
    
    // Method closeStatement is also bound to symbol ";"    
    RXObject_setSlot(RXProtoObject_o, RXSymbol_semicolon_o,
        RXNativeMethod_attach(RXProtoObject, closeStatement)
    );
    
    RXSymbol_Expression_o = RXSymbol_symbolForCString("Expression");
    RXObject_setSlot(RXLobby_o, RXSymbol_Expression_o, RXExpression_o);
}
