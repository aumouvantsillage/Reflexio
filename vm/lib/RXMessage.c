
#include "RXLib.h"

RXObject_t* RXMessage_o;
RXObject_t* RXSymbol_Message_o;
RXObject_t* RXSymbol_name_o;
RXObject_t* RXSymbol_valueOnObjectInContext_o;

static RXNativeMethod_define(RXMessage, valueOnObjectInContext) {
    // Push arguments to the stack
    Eina_List* iter;
    RXObject_t* arg;
    EINA_LIST_REVERSE_FOREACH(*(Eina_List**)self, iter, arg) {
        RXNativeMethod_push(arg);
    }
    
    // Send message to receiver
    return RXObject_respondTo(
        RXExpression_valueOfArgumentAt(0, context),
        RXObject_valueOfSlot(self, RXSymbol_name_o),
        RXExpression_valueOfArgumentAt(1, context),
        eina_list_count(*(Eina_List**)self)
    );
}

void RXMessage_setup(void) {
    RXSymbol_name_o = RXSymbol_symbolForCString("name");
    RXSymbol_valueOnObjectInContext_o = RXSymbol_symbolForCString("valueOnObjectInContext");
    
    RXMessage_o = RXList_spawn(RXList_o);
    
    // The common message delegate has "nil" as message name
    // and and empty argument list
    RXObject_setSlot(RXMessage_o, RXSymbol_name_o, RXSymbol_nil_o);

    RXNativeMethod_attach(RXMessage, valueOnObjectInContext);
    
    RXSymbol_Message_o = RXSymbol_symbolForCString("Message");
    RXObject_setSlot(RXLobby_o, RXSymbol_Message_o, RXMessage_o);
}
