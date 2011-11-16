
#include "RXLib.h"

RXObject_t* RXSymbol_valueOnObjectInContext_o;
RXObject_t* RXSymbol_name_o;
RXObject_t* RXSymbol_arguments_o;

static RXNativeMethod_define(RXMessage, valueOnObjectInContext) {
    // Push arguments to the stack
    RXObject_t* args = RXObject_valueOfSlot(self, RXSymbol_arguments_o);
    Eina_List* iter;
    RXObject_t* data;
    EINA_LIST_REVERSE_FOREACH(*(Eina_List**)args, iter, data) {
        RXNativeMethod_push(data);
    }
    
    // Send message to receiver
    return RXObject_respondTo(
        RXObject_valueOfArgumentAt(0, context),
        RXObject_valueOfSlot(self, RXSymbol_name_o),
        RXObject_valueOfArgumentAt(1, context),
        eina_list_count(*(Eina_List**)args)
    );
}

RXObject_t* RXMessage_o;

void RXMessage_setup(void) {
    RXMessage_o = RXList_spawn(RXList_o);

    RXSymbol_name_o = RXSymbol_symbolForCString("name");
    RXSymbol_arguments_o = RXSymbol_symbolForCString("arguments");
    RXSymbol_valueOnObjectInContext_o = RXSymbol_symbolForCString("valueOnObjectInContext");
    
    RXNativeMethod_attach(RXMessage, valueOnObjectInContext);
}
