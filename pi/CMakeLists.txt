project(vsnLib)
cmake_minimum_required(VERSION 3.4)

SET(CMAKE_CXX_STANDARD 17)
add_definitions(-fPIC)


include_directories( 
    inc
    )

FILE(GLOB  SRC_VSNLIB src/*.cpp)
add_library(piLib ${SRC_VSNLIB})
target_link_libraries(piLib   
    )
    
set(ENV{embLib_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  

#---- vsnTest
FILE(GLOB  SRC_VSNTEST test/*.cpp)
add_executable(vsnTest ${SRC_VSNTEST})
target_link_libraries(vsnTest  
    vsnLib
    ${OpenCV_LIBRARIES} 
    )

#---- vsnTool
FILE(GLOB  SRC_VSNTEST apps/*.cpp)
add_executable(vsntool ${SRC_VSNTEST})
target_link_libraries(vsntool  
    ${OpenCV_LIBRARIES} 
    vsnLib)

#---- calib tool
add_subdirectory(calib)