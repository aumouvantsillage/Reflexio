
#include <Reflexio.h>
#include <parser/RXParser.h>

int main(int argc, char* argv[]) {
    FILE* file;
    if (argc > 1) {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "Failed to open file for reading.\n");
            return 0;
        }
    }
    else {
        fprintf(stderr, "Please provide a file name.\n");
        return 0;
    }
    
    Reflexio_setup(argc - 1, argv + 1);
    RXObject_t* expression = RXParser_expressionFromCFile(file);
    RXNativeMethod_push(RXLobby_o);
    RXObject_respondTo(expression, RXSymbol_valueInContext_o, RXLobby_o, 1);
    Reflexio_clean();
    return -1;
}
