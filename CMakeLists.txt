cmake_minimum_required (VERSION 2.8) 
project(Reflectio)

option(DEBUG "Add debug info and evaluate assertions" ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-extensions/ )

add_definitions(-std=gnu99 -D_GNU_SOURCE)

find_package(Eina)
include_directories(${EINA_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/vm ${PROJECT_SOURCE_DIR}/vm )
set(LIBS ${EINA_LIBRARIES} antlr3c reflexio)

if(DEBUG)
    # Add debug info, execute asserts
    add_definitions(-g)
else(DEBUG)
    # No debug info, do not execute asserts
    add_definitions(-DNDEBUG)
endif(DEBUG)

add_subdirectory(vm)
add_subdirectory(tools)
add_subdirectory(test)
