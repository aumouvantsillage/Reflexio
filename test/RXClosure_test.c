
#include <Reflexio.h>

void main(void) {
    Reflexio_setup();
    
    // lobby = Object spawn
    RXObject_t* lobby = RXObject_spawn(RXObject_o);
    
    // lobby person = Object spawn
    RXObject_t* person = RXObject_spawn(RXObject_o);
    RXObject_setSlot(lobby, RXSymbol_symbolForCString("person"), person);
    
    // lobby albert = lobby person spawn
    RXObject_t* albert = RXObject_spawn(person);
    RXObject_setSlot(lobby, RXSymbol_symbolForCString("albert"), albert);

    // lobby hello = "Salut"
    RXObject_setSlot(lobby, RXSymbol_symbolForCString("hello"), RXSymbol_symbolForCString("Salut "));
    
    // lobby albert name = "Albert"
    RXObject_setSlot(albert, RXSymbol_name_o, RXSymbol_symbolForCString("Albert"));
    
    // lobby albert println
    // -> Object_0x...
    printf("With default println -> ");
    RXObject_respondTo(albert, RXSymbol_println_o, lobby, 0);
    
    // lobby person println = lobby method(hello print; self name println)
    RXObject_t* printlnBody = RXList_spawn(RXExpression_o);
    RXObject_t* msg;
    
    msg = RXList_spawn(RXMessage_o);
    RXObject_setSlot(msg, RXSymbol_name_o, RXSymbol_symbolForCString("hello"));
    RXList_append(printlnBody, msg);
    
    msg = RXList_spawn(RXMessage_o);
    RXObject_setSlot(msg, RXSymbol_name_o, RXSymbol_print_o);
    RXList_append(printlnBody, msg);

    msg = RXList_spawn(RXMessage_o);
    RXObject_setSlot(msg, RXSymbol_name_o, RXSymbol_semicolon_o);
    RXList_append(printlnBody, msg);

    msg = RXList_spawn(RXMessage_o);
    RXObject_setSlot(msg, RXSymbol_name_o, RXSymbol_self_o);
    RXList_append(printlnBody, msg);

    msg = RXList_spawn(RXMessage_o);
    RXObject_setSlot(msg, RXSymbol_name_o, RXSymbol_name_o);
    RXList_append(printlnBody, msg);

    msg = RXList_spawn(RXMessage_o);
    RXObject_setSlot(msg, RXSymbol_name_o, RXSymbol_println_o);
    RXList_append(printlnBody, msg);
        
    RXNativeMethod_push(printlnBody);
    RXObject_t* println = RXObject_respondTo(lobby, RXSymbol_method_o, lobby, 1);
    
    RXObject_setSlot(person, RXSymbol_println_o, println);
    
    // lobby albert println
    // -> Salut Albert
    printf("With custom println -> ");
    RXObject_respondTo(albert, RXSymbol_println_o, lobby, 0);
    
    Reflexio_clean();
}
