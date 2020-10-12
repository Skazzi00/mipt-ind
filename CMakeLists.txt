cmake_minimum_required(VERSION 3.17)
project(mipt_ind)

set(CMAKE_CXX_STANDARD 17)
set(COMMON_FLAGS "-Werror -Wall -Wextra -pedantic -Wshadow -Wconversion -Wsign-conversion  -Wignored-qualifiers -Wwrite-strings -DFAST_IOSTREAM")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++17")
set(CMAKE_C_FLAGS ${COMMON_FLAGS})

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

target_link_libraries(squareC miptLib)

add_executable(format_file src/format_file.cpp)

add_executable(stack_ex src/stack_ex.c)

target_link_libraries(stack_ex miptLib)

add_executable(CheckedStack src/CheckedStack.cpp)