cmake_minimum_required(VERSION 3.10)


project(game1 LANGUAGES CXX)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

set(SOURCES
    ${SRC_DIR}/main.cpp
)


add_library(pers SHARED ${SRC_DIR}/pers.cpp)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")


add_executable(main ${SOURCES})
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system)
target_link_libraries(pers PRIVATE sfml-graphics sfml-window sfml-system)
target_include_directories(main PRIVATE ${SRC_DIR}/pers.h)
target_link_libraries(main PRIVATE pers)


set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")



