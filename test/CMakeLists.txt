
find_package(Eina)

include_directories(${EINA_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/vm )

set(LIBS ${EINA_LIBRARIES} gc reflexio)

add_executable(RXObject_slots_test RXObject_slots_test.c)
target_link_libraries(RXObject_slots_test ${LIBS})

add_executable(RXNumber_test RXNumber_test.c)
target_link_libraries(RXNumber_test ${LIBS})

add_executable(RXList_test RXList_test.c)
target_link_libraries(RXList_test ${LIBS})
