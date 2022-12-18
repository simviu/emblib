project(emblib)
cmake_minimum_required(VERSION 3.4)

SET(CMAKE_CXX_STANDARD 14)

set(BUILD_TESTS FALSE)
add_subdirectory(dep/CppLinuxSerial)


include_directories( 
    inc 
    dep/CppLinuxSerial/include
    )


set(ENV{embLib_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  

FILE(GLOB  SRC_EMBLIB src/*.cpp)
add_library(emblib ${SRC_EMBLIB})
target_link_libraries(emblib  
    CppLinuxSerial
    )
    