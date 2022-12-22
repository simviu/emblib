project(emblib)
cmake_minimum_required(VERSION 3.4)

SET(CMAKE_CXX_STANDARD 17)

#set(BUILD_TESTS FALSE)
#add_subdirectory(dep/CppLinuxSerial)


include_directories( 
    ${JsonCpp_INCLUDE_DIR}
    /usr/include/jsoncpp
    inc
    )

#--- Hack for Mac brew
message(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
link_directories(
    /opt/dep/local/lib
    /usr/local/lib
    ${CMAKE_PREFIX_PATH}/lib)

    
#---- utLib 
FILE(GLOB  SRC_UTLIB src/ut/*.cpp)
add_library(utLib ${SRC_UTLIB})
target_link_libraries(utLib   
    ${JsonCpp_LIBRARIES} 
    jsoncpp
    pthread
    )
    
    

include_directories( 
    dep/CppLinuxSerial/include
    )


set(ENV{embLib_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  

FILE(GLOB  SRC_EMBLIB src/*.cpp)
add_library(emblib ${SRC_EMBLIB})
target_link_libraries(emblib  
    CppLinuxSerial
    )
    
