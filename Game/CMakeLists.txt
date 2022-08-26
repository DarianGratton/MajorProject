# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
cmake_minimum_required (VERSION 3.8)

project ("Game")

# Setup include directory
set(INCLUDE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/include")
message(STATUS "Using include directory: ${INCLUDE_DIR}")
include_directories(${INCLUDE_DIR})

# Include sub-projects
add_subdirectory("src")
add_subdirectory("test")
