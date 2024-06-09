cmake_minimum_required(VERSION 3.24)
project(FunctionTraits)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -lgomp -pthread -O2 -save-temps=obj -ftemplate-depth=4096")

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.c ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.c ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp)

add_library(${PROJECT_NAME} OBJECT ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
