
find_package(Eina)

include_directories(${EINA_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/vm )

set(LIBS ${EINA_LIBRARIES} reflexio)

add_executable(rxrun rxrun.c)
target_link_libraries(rxrun ${LIBS})
