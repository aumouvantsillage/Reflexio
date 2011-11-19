
#include <Reflexio.h>

void main(void) {
    Reflexio_setup(0, NULL);
    
    RXObject_t* source = RXSymbol_symbolForCString(
        "person = Object spawn \n\
        albert = person spawn \n\
        hello = \"Salut \" \n\
        albert name = \"Albert\" \n\
        albert println \n\
        person println = method( \n\
            hello print \n\
            self name println \n\
        ) \n\
        albert println \n"
    );
    
    // printf((char*)source);
    // printf((char*)RXObject_respondTo(source, RXSymbol_symbolForCString("asSource"), RXLobby_o, 0));
    
    RXNativeMethod_push(source);
    RXObject_t* expression = RXObject_respondTo(RXExpression_o, RXSymbol_fromString_o, RXLobby_o, 1);
    printf("%s\n", RXObject_respondTo(expression, RXSymbol_symbolForCString("asSource"), RXLobby_o, 0));

    RXNativeMethod_push(RXLobby_o);
    RXObject_respondTo(expression, RXSymbol_valueInContext_o, RXLobby_o, 1);
    
    Reflexio_clean();
}
