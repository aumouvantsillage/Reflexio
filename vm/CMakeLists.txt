
set(VM_SRC
    Reflexio.c
    core/RXCore.c
    core/RXObject.c
    core/RXSymbol.c
    core/RXNativeMethod.c
    core/RXCache.c
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
    OUTPUT parser/RXParser.c
    DEPENDS parser/RXParser.y
    COMMAND bison --report=state --defines=${CMAKE_CURRENT_BINARY_DIR}/parser/RXTokens.h -o ${CMAKE_CURRENT_BINARY_DIR}/parser/RXParser.c ${CMAKE_CURRENT_SOURCE_DIR}/parser/RXParser.y
)

add_custom_command(
    OUTPUT parser/RXLexer.c
    DEPENDS parser/RXLexer.l parser/RXParser.c
    COMMAND flex -o ${CMAKE_CURRENT_BINARY_DIR}/parser/RXLexer.c ${CMAKE_CURRENT_SOURCE_DIR}/parser/RXLexer.l
)

find_package(Eina)
include_directories(${EINA_INCLUDE_DIRS} .)

add_library(reflexio SHARED ${VM_SRC} )
