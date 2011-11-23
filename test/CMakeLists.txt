
add_executable(RXObject_slots_test RXObject_slots_test.c)
target_link_libraries(RXObject_slots_test ${LIBS})

add_executable(RXNumber_test RXNumber_test.c)
target_link_libraries(RXNumber_test ${LIBS})

add_executable(RXList_test RXList_test.c)
target_link_libraries(RXList_test ${LIBS})

add_executable(RXClosure_test RXClosure_test.c)
target_link_libraries(RXClosure_test ${LIBS})

add_executable(RXParser_test RXParser_test.c)
target_link_libraries(RXParser_test ${LIBS})
