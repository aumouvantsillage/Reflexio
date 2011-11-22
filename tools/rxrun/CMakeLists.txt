
find_package(Eina)

include_directories(${EINA_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/vm ${PROJECT_SOURCE_DIR}/vm )

set(LIBS ${EINA_LIBRARIES} antlr3c reflexio)

add_executable(rxrun rxrun.c)
target_link_libraries(rxrun ${LIBS})
