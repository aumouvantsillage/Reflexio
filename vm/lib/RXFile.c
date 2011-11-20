
#include "RXLib.h"

// Private -------------------------------------------------------------

RXObject_defineType(RXFile_t, FILE*);

#define RXFile_payload(self) ((RXFile_t*)self)->payload

static RXObject_t* RXFile_new(FILE* f) {
    RXObject_t* result = RXObject_allocateType(RXFile_t);
    RXObject_initialize(result);
    RXFile_payload(result) = f;
    return result;
}

static RXObject_t* RXFile_open(const char* fileName, const char* flags) {
    FILE* fileHandle = fopen(fileName, flags);
    if (fileHandle != NULL) {
        RXObject_t* result = RXFile_new(fileHandle);
        RXObject_setSlot(result, RXSymbol_delegate_o, RXFile_o);
        return result;
    }
    else {
        return RXNil_o;
    }
}

// Methods -------------------------------------------------------------

RXNativeMethod_define(RXFile, spawn) {
    RXObject_t* result = RXFile_new(RXFile_payload(self));
    RXObject_setSlot(result, RXSymbol_delegate_o, self);
    return result;
}

/*
 * Spawns a new file opened for reading.
 *
 * Arguments:
 *  - the path of the file to open
 *
 * Returns:
 *  - a new file object, nil on failure
 */
RXNativeMethod_define(RXFile, openForReading) {
    RXObject_t* fileName = RXExpression_valueOfArgumentAt(0, context);
    return RXFile_open((char*)fileName, "r");
}

/*
 * Spawns a new file opened for writing.
 *
 * Arguments:
 *  - the path of the file to open
 *
 * Returns:
 *  - a new file object, nil on failure
 */
RXNativeMethod_define(RXFile, openForWriting) {
    RXObject_t* fileName = RXExpression_valueOfArgumentAt(0, context);
    return RXFile_open((char*)fileName, "w");
}

/*
 * Close the current file.
 */
RXNativeMethod_define(RXFile, close) {
    fclose(RXFile_payload(self));
}

// Public --------------------------------------------------------------

RXObject_t* RXFile_o;
RXObject_t* RXSymbol_File_o;

void RXFile_setup(void) {
    RXFile_o = RXFile_new(NULL);
    RXObject_setSlot(RXFile_o, RXSymbol_delegate_o, RXObject_o);
    
    RXNativeMethod_attach(RXFile, spawn);
    RXNativeMethod_attach(RXFile, openForReading);
    RXNativeMethod_attach(RXFile, openForWriting);
    RXNativeMethod_attach(RXFile, close);

    RXSymbol_File_o = RXSymbol_symbolForCString("File");
    RXObject_setSlot(RXLobby_o, RXSymbol_File_o, RXFile_o);
}
