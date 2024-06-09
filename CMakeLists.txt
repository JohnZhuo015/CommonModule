cmake_minimum_required(VERSION 3.24)
project(Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -lgomp -pthread -O2 -save-temps=obj -ftemplate-depth=4096")

find_package(Boost COMPONENTS thread date_time filesystem system REQUIRED)

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/source/*.c ${CMAKE_SOURCE_DIR}/source/*.cc ${CMAKE_SOURCE_DIR}/source/*.cpp)
file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.c ${CMAKE_SOURCE_DIR}/include/*.cc ${CMAKE_SOURCE_DIR}/include/*.cpp)
file(GLOB UI_FILES ${CMAKE_SOURCE_DIR}/ui/*.ui)
file(GLOB RESOURCE_FILE ${CMAKE_SOURCE_DIR}/resource/*.qrc)

add_subdirectory(module/FunctionTraits)
add_subdirectory(module/OverloadLambda)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES} ${RESOURCE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PUBLIC FunctionTraits)
target_link_libraries(${PROJECT_NAME} PUBLIC OverloadLambda)