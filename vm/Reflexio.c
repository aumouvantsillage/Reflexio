
#include "Reflexio.h"

void Reflexio_setup(int argc, char* argv[]) {
    eina_init();
    
    RXCore_setup();
    
    RXObject_libSetup();
    RXSymbol_libSetup();
    
    RXBoolean_setup();
    RXNumber_setup();
    RXInteger_setup();
    RXList_setup();
    RXMessage_setup();
    RXExpression_setup();
    RXClosure_setup();
    RXFile_setup();
    
    // Copy argument list into the Lobby objet
    RXObject_t* argumentList = RXList_spawn(RXList_o, NULL);
    for (int i = 1; i < argc; i ++) {
        RXList_append(argumentList, RXSymbol_symbolForCString(argv[i]));
    }
    RXObject_setSlot(RXLobby_o, RXSymbol_arguments_o, argumentList);
}

void Reflexio_clean(void) {
    RXCore_clean();

    eina_shutdown();
}
