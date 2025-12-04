cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 0.1.0 LANGUAGES C CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ============================================================================
# EVALUATION 3: Threading Support - ADD THESE LINES
# ============================================================================
# Enable threading support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Compiler directive to enable/disable threading
# Usage: cmake -DENABLE_THREADING=ON ..  (default: ON)
#        cmake -DENABLE_THREADING=OFF ..
option(ENABLE_THREADING "Enable multithreading" ON)

if(ENABLE_THREADING)
    add_compile_definitions(USE_THREADING)
    message(STATUS "Threading: ENABLED")
else()
    message(STATUS "Threading: DISABLED")
endif()
# ============================================================================

add_executable(raytracer main.cpp)

# Give the main executable a clear name for tests to find
set_property(TARGET raytracer PROPERTY OUTPUT_NAME "raytracer")


target_include_directories(raytracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/src/raymath"
                           "${PROJECT_SOURCE_DIR}/src/rayimage"
                           "${PROJECT_SOURCE_DIR}/src/rayscene"
                           )

add_subdirectory(./src/raymath)
add_subdirectory(./src/rayimage)
add_subdirectory(./src/rayscene)
add_subdirectory(./src/lodepng)

target_link_libraries(raytracer 
                      PUBLIC 
                      rayscene
                      raymath
                      rayimage
                      lodepng
                      Threads::Threads  # ← ADD THIS LINE for threading
                      )

# --- Testing ---
# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(
  raytracer_tests
  ./src/tests/e2e_tests.cpp
)

# Pass the location of the main executable to the tests
target_compile_definitions(raytracer_tests PRIVATE
    RAYTRACER_EXECUTABLE="$<TARGET_FILE:raytracer>"
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

target_link_libraries(raytracer_tests PRIVATE gtest_main lodepng rayimage)

include(GoogleTest)
gtest_discover_tests(raytracer_tests)
add_subdirectory(./src/tests)