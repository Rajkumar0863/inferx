cmake_minimum_required(VERSION 3.20)

project(
    InferX
    VERSION 0.4.0
    DESCRIPTION "AI Inference Request Scheduler Simulator"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    inferx
    src/main.cpp
    src/inference_request.cpp
    src/fifo_scheduler.cpp
    src/priority_scheduler.cpp
    src/batch.cpp
    src/dynamic_batcher.cpp
)

target_include_directories(
    inferx
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)