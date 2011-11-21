
#include "RXLib.h"

// Methods -------------------------------------------------------------

RXNativeMethod_define(RXObject, valueOnObjectInContext) {
    return self;
}

RXNativeMethod_define(RXMessage, valueOnObjectInContext) {
    RXObject_t* receiver = RXExpression_valueOfArgumentAt(0, context);
    RXObject_t* messageName = RXObject_valueOfSlot(self, RXSymbol_name_o);
    RXObject_t* msgContext = RXExpression_valueOfArgumentAt(1, context);
    int msgArgumentCount = eina_list_count(*(Eina_List**)self);
    
    // Push arguments to the stack
    Eina_List* iter;
    RXObject_t* arg;
    EINA_LIST_REVERSE_FOREACH(*(Eina_List**)self, iter, arg) {
        RXNativeMethod_push(arg);
    }
    
    // Send message to receiver
    return RXObject_respondTo(receiver, messageName, msgContext, msgArgumentCount);
}

RXNativeMethod_define(RXMessage, asSource) {
    RXObject_t* name = RXObject_valueOfSlot(self, RXSymbol_name_o);
    int len = strlen((char*)name)+3; // name(...)\0
    
    int count = RXList_count(self);
    RXObject_t* msgSrc[count];
    
    int index = 0;
    Eina_List* iter;
    RXObject_t* msg;
    EINA_LIST_FOREACH(*(Eina_List**)self, iter, msg) {
        RXObject_t* src = RXObject_respondTo(msg, RXSymbol_asSource_o, RXLobby_o, 0);
        len += strlen((char*)src);
        if (index) {
            len += 2; // arg preceded by comma and space
        } 
        msgSrc[index++] = src;
    }
    char result[len];
    strcpy(result, (char*)name);

    if (count) {
        strcat(result, "(");
        for (index = 0; index < count; index ++) {
            if (index) {
                strcat(result, ", ");
            }
            strcat(result, (char*)(msgSrc[index]));
        }
        strcat(result, ")");
    }
    return RXSymbol_symbolForCString(result);
}

// Public --------------------------------------------------------------

RXObject_t* RXMessage_o;
RXObject_t* RXSymbol_Message_o;
RXObject_t* RXSymbol_name_o;
RXObject_t* RXSymbol_valueOnObjectInContext_o;

void RXMessage_setup(void) {
    RXSymbol_name_o = RXSymbol_symbolForCString("name");
    RXSymbol_valueOnObjectInContext_o = RXSymbol_symbolForCString("valueOnObjectInContext");
    
    RXMessage_o = RXList_spawn(RXList_o, NULL);
    
    // The common message delegate has "nil" as message name
    // and and empty argument list
    RXObject_setSlot(RXMessage_o, RXSymbol_name_o, RXSymbol_nil_o);

    RXNativeMethod_attach(RXObject, valueOnObjectInContext);
    RXNativeMethod_attach(RXMessage, valueOnObjectInContext);
    RXNativeMethod_attach(RXMessage, asSource);
    
    RXSymbol_Message_o = RXSymbol_symbolForCString("Message");
    RXObject_setSlot(RXLobby_o, RXSymbol_Message_o, RXMessage_o);
}
