
#include "RXLib.h"

// Methods -------------------------------------------------------------

RXNativeMethod_define(RXSymbol, asString) {
    return self;
}

RXNativeMethod_define(RXSymbol, asSource) {
    char* cstr = (char*)self;
      
    // Compute string length, including quotes and backslashes for escaped characters
    int len = 3; // ""\0
    for(char* ptr = cstr; *ptr; ptr++) {
        if (strchr("\n\r\t\"\'\\", *ptr))
            len ++;
        len ++;
    }
      
    // Copy string, escaping special characters
    char* estr = malloc(len);
    char* eptr = estr;
    *eptr++ = '\"';
    for (char* cptr = cstr; *cptr; cptr++) {
        switch(*cptr) {
            case '\n': *eptr++ = '\\'; *eptr++ = 'n'; break;
            case '\r': *eptr++ = '\\'; *eptr++ = 'r'; break;
            case '\t': *eptr++ = '\\'; *eptr++ = 't'; break;
            case '\"': *eptr++ = '\\'; *eptr++ = '\"'; break;
            case '\'': *eptr++ = '\\'; *eptr++ = '\''; break;
            case '\\': *eptr++ = '\\'; *eptr++ = '\\'; break;
            default:   *eptr++ = *cptr;
        }
    }
    *eptr++ = '\"';
    *eptr = 0;
    
    RXObject_t* result = RXSymbol_symbolForCString(estr);
    free(estr);
    return result;
}

RXNativeMethod_define(RXSymbol, print) {
    fputs((char*)self, stdout);
    return self;
}

RXNativeMethod_define(RXSymbol, println) {
    puts((char*)self);
    return self;
}

// Public --------------------------------------------------------------

void RXSymbol_libSetup(void) {
    RXNativeMethod_attach(RXSymbol, asString);
    RXNativeMethod_attach(RXSymbol, asSource);
    RXNativeMethod_attach(RXSymbol, print);
    RXNativeMethod_attach(RXSymbol, println);

    RXObject_setSlot(RXLobby_o, RXSymbol_o, RXSymbol_o);
}
