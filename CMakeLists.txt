
project(cpp-exercices)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

file(GLOB_RECURSE SRC_FILES   "src/*.cpp")
file(GLOB_RECURSE TESTS_FILES "tests/*.cpp")

add_executable(tests
    ${SRC_FILES}
    ${TESTS_FILES})

target_include_directories(tests PUBLIC
    "include"
    "deps/catch/include")

add_test(tests "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests")
