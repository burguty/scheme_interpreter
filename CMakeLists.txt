cmake_minimum_required(VERSION 3.20)
project(scheme_interpreter LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SCHEME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SCHEME_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SCHEME_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)

# LIBRARY
add_library(
    scheme
    src/applier.cpp
    src/applier_integer.cpp
    src/applier_boolean.cpp
    src/applier_list.cpp
    src/applier_quote.cpp
    src/object.cpp
    src/parser.cpp
    src/scheme.cpp
    src/tokenizer.cpp
)

target_compile_features(
    scheme
    PUBLIC
        cxx_std_23
)

target_include_directories(
    scheme
    PUBLIC
        ${SCHEME_INCLUDE_DIR}
    PRIVATE
        ${SCHEME_SOURCE_DIR}
)


# TESTS
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.11.0
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(
    scheme_tests
    tests/test_boolean.cpp
    tests/test_eval.cpp
    tests/test_fuzzing_1.cpp
    tests/test_fuzzing_2.cpp
    tests/test_integer.cpp
    tests/test_list.cpp
    tests/test_parser.cpp
    tests/test_tokenizer.cpp
)

target_link_libraries(
    scheme_tests
    PRIVATE
        scheme
        Catch2::Catch2WithMain
)

target_include_directories(
    scheme_tests
    PRIVATE
        ${SCHEME_SOURCE_DIR}
        ${SCHEME_COMMON_DIR}
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(scheme_tests)


# REPL
add_executable(scheme_repl repl/main.cpp)
target_link_libraries(
    scheme_repl
    PRIVATE
        scheme
)