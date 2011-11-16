
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
)

find_package(Eina)
include_directories(${EINA_INCLUDE_DIRS} .)

add_library(reflexio SHARED ${VM_SRC} )
