
#include <Reflexio.h>

void main(void) {
    Reflexio_setup(0, NULL);
    
    // Lobby person = Object spawn
    RXObject_t* person = RXObject_spawn(RXObject_o);
    RXObject_setSlot(RXLobby_o, RXSymbol_symbolForCString("person"), person);
    
    // Lobby albert = Lobby person spawn
    RXObject_t* albert = RXObject_spawn(person);
    RXObject_setSlot(RXLobby_o, RXSymbol_symbolForCString("albert"), albert);

    // Lobby hello = "Salut"
    RXObject_setSlot(RXLobby_o, RXSymbol_symbolForCString("hello"), RXSymbol_symbolForCString("Salut "));
    
    // Lobby albert name = "Albert"
    RXObject_setSlot(albert, RXSymbol_name_o, RXSymbol_symbolForCString("Albert"));
    
    // Lobby albert println
    // -> Object_0x...
    printf("With default println -> ");
    RXObject_respondTo(albert, RXSymbol_println_o, RXLobby_o, 0);
    
    // Lobby person println = Lobby method(hello print; self name println)
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
    RXObject_t* println = RXObject_respondTo(RXLobby_o, RXSymbol_method_o, RXLobby_o, 1);
    
    RXObject_setSlot(person, RXSymbol_println_o, println);
    
    // Lobby albert println
    // -> Salut Albert
    printf("With custom println -> ");
    RXObject_respondTo(albert, RXSymbol_println_o, RXLobby_o, 0);
    
    Reflexio_clean();
}
