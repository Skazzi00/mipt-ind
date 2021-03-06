cmake_minimum_required(VERSION 3.17)
project(mipt_ind)

set(CMAKE_CXX_STANDARD 17)
set(COMMON_FLAGS " -Werror -Wall -Wextra -pedantic -Wshadow -Wconversion -Wsign-conversion  -Wignored-qualifiers -Wwrite-strings -DFAST_IOSTREAM -fsanitize=leak -fsanitize=address -fsanitize=undefined -fPIE -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++17")
set(CMAKE_C_FLAGS ${COMMON_FLAGS})

set(SFML_INCLUDE_DIR "/usr/local/Cellar/sfml/2.5.1/include")
set(SFML_LIBRARY_DIR "/usr/local/Cellar/sfml/2.5.1/lib")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)


set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

file(GLOB LIB_FILES src/lib/*.c)

add_library(miptLib ${LIB_FILES})

add_subdirectory(./googletest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(runUnitTests ${TEST_SRC_FILES})

# Standard linking to gtest stuff
target_link_libraries(runUnitTests gtest gtest_main miptLib)

add_executable(square src/square.cpp)

add_executable(sort src/sort.cpp)

add_executable(sortC src/sort.c)

target_link_libraries(sortC miptLib)

add_executable(squareC src/square.c)

target_link_libraries(squareC miptLib m)

add_executable(format_file src/format_file.cpp)

add_executable(processor src/processor.cpp)

target_link_libraries(processor miptLib sfml-graphics sfml-window sfml-system)

add_executable(vassembler src/vassembler.cpp)

target_link_libraries(vassembler miptLib)

add_executable(vdisassembler src/vdisassembler.cpp)

target_link_libraries(vdisassembler miptLib)

add_executable(akinator src/akinator.cpp)

target_link_libraries(akinator miptLib)

