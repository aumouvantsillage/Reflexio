
set(VM_SRC
    Reflexio.c
    core/RXCore.c
    core/RXObject.c
    core/RXSymbol.c
    core/RXNativeMethod.c
    lib/RXObject.c
)

find_package(Eina)
include_directories(${EINA_INCLUDE_DIRS} .)

add_library(reflexio SHARED ${VM_SRC} )
