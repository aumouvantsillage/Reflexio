cmake_minimum_required (VERSION 2.8) 
project(Reflectio)

set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake-extensions/ )

add_definitions(-std=gnu99 -D_GNU_SOURCE)

add_subdirectory(vm)
add_subdirectory(tools)
add_subdirectory(test)
