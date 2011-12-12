
set(VM_SRC
    Reflexio.c
    core/RXCore.c
    core/RXObject.c
    core/RXSymbol.c
    core/RXNativeMethod.c
    lib/RXObject.c
    lib/RXSymbol.c
    lib/RXBoolean.c
    lib/RXNumber.c
    lib/RXInteger.c
    lib/RXList.c
    lib/RXMessage.c
    lib/RXExpression.c
    lib/RXClosure.c
    lib/RXFile.c
    parser/RXParser.c
    parser/RXLexer.c
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/parser)

add_custom_command(
    OUTPUT parser/RXParser.h parser/RXLexer.h parser/RXParser.c parser/RXLexer.c
    DEPENDS parser/RX.g
    COMMAND antlr3 -o ${CMAKE_CURRENT_BINARY_DIR}/parser ${CMAKE_CURRENT_SOURCE_DIR}/parser/RX.g
)

add_library(reflexio SHARED ${VM_SRC} )
