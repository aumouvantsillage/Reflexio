
#include <Reflexio.h>
#include <parser/RXParser.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Please provide a file name.\n");
        return 0;
    }
    
    Reflexio_setup(argc - 1, argv + 1);
    RXObject_t* expression = RXParser_expressionFromCFile(argv[1]);
    RXNativeMethod_push(RXLobby_o);
    RXObject_respondTo(expression, RXSymbol_valueInContext_o, RXLobby_o, 1);
    Reflexio_clean();
    return -1;
}
