cmake_minimum_required(VERSION 3.17)
project(mipt_ind)

set(CMAKE_CXX_STANDARD 17)
set(COMMON_FLAGS "-Werror -Wall -Wextra -pedantic -Wshadow -Wconversion -Wsign-conversion -fsanitize=address -Wignored-qualifiers -Wwrite-strings -DFAST_IOSTREAM")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++17")
set(CMAKE_C_FLAGS ${COMMON_FLAGS})

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

file(GLOB LIB_FILES src/C/lib/*.c)

add_library(miptLib ${LIB_FILES})

add_subdirectory(./googletest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/CPP/*.cpp)

add_executable(runUnitTests ${TEST_SRC_FILES})

# Standard linking to gtest stuff
target_link_libraries(runUnitTests gtest gtest_main miptLib)

add_executable(square src/CPP/square.cpp)

add_executable(sort src/CPP/sort.cpp)

add_executable(sortC src/C/sort.c)

target_link_libraries(sortC miptLib)

add_executable(squareC src/C/square.c)

target_link_libraries(squareC miptLib)

add_executable(format_file src/CPP/format_file.cpp)

add_executable(stack_example src/C/stack_ex.c)

target_link_libraries(stack_example miptLib)