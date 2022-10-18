cmake_minimum_required(VERSION 3.14)

find_package(Torch REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Setup include directory
set(INCLUDE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/include")
message(STATUS "Using include directory: ${INCLUDE_DIR}")
include_directories(${INCLUDE_DIR})

add_executable (
	AgentTests
	"Test.cpp"
	"EnvironmentTests.h"
	"StateTests.h"
	"StorageTests.h"
	"../src/Environment/Environment.h" 
    "../src/Environment/Environment.cpp"
    "../src/Environment/State.h" 
    "../src/Environment/State.cpp"
	"../src/Storage/UtilityStorage.h" 
    "../src/Storage/UtilityStorage.cpp"
)

# Setup Google Test
include(FetchContent)
FetchContent_Declare (
	googletest
	URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET AgentTests
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:AgentTests>)
endif (MSVC)

# Link Libraries
target_link_libraries(AgentTests gtest_main)
target_link_libraries(AgentTests "${TORCH_LIBRARIES}")

# Include
include(GoogleTest)
gtest_discover_tests(AgentTests)

# Set properties
set_property(TARGET AgentTests PROPERTY CXX_STANDARD 17)
