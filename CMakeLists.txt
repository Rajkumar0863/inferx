cmake_minimum_required(VERSION 3.20)

project(
    InferX
    VERSION 0.8.0
    DESCRIPTION "AI Inference Request Scheduler Simulator"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

set(
    gtest_force_shared_crt
    ON
    CACHE BOOL ""
    FORCE
)

FetchContent_MakeAvailable(googletest)

add_library(
    inferx_core
    src/inference_request.cpp
    src/fifo_scheduler.cpp
    src/priority_scheduler.cpp
    src/batch.cpp
    src/dynamic_batcher.cpp
    src/worker_pool.cpp
    src/metrics.cpp
)

target_include_directories(
    inferx_core
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(
    inferx
    src/main.cpp
)

target_link_libraries(
    inferx
    PRIVATE
    inferx_core
)

enable_testing()

add_executable(
    inferx_tests
    tests/test_fifo_scheduler.cpp
    tests/test_priority_scheduler.cpp
    tests/test_dynamic_batcher.cpp
    tests/test_metrics.cpp
)

target_link_libraries(
    inferx_tests
    PRIVATE
    inferx_core
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(
    inferx_tests
)