cmake_minimum_required(VERSION 3.20)

project(
    InferX
    VERSION 0.1.0
    DESCRIPTION "AI Inference Request Scheduler Simulator"
    LANGUAGES CXX
)

# Use the C++20 standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build the InferX executable.
add_executable(
    inferx
    src/main.cpp
)

# Allow source files to include headers from include/.
target_include_directories(
    inferx
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)