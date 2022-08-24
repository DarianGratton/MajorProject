# CMakeList.txt : CMake project for Agent, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.14)

find_package(Torch REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Add source to this project's executable.
add_executable (
    Agent 
    "Main.cpp" 
    "Agent.cpp" 
    "Agent.h" 
    "Environment/Environment.h" 
    "Environment/Environment.cpp"
    "Environment/State.h" 
    "Environment/State.cpp"
    "Networks/TestEnvironment/TestEnvironment.h" 
    "Networks/TestEnvironment/TestEnvironment.cpp"
    "Networks/TestEnvironment/AgentTest.h"
    "Networks/TestEnvironment/AgentTest.cpp"
    "Networks/ACER/RandomIterator.h"  
    "Networks/ACER/ReplayMemory.h" 
    "Networks/ACER/ReplayMemory.cpp" 
    "Networks/ACER/ActorCriticNetwork.h" 
    "Networks/ACER/ActorCriticNetwork.cpp" 
    "Networks/ACER/ACERAgent.cpp" 
    "Networks/ACER/Trajectory.h" 
    "Networks/NetworkAgent.h" 
    "Networks/NetworkParameters.h" 
    "Networks/ACER/ACERParameters.h" 
    "Networks/AgentFactory.h" 
    "Networks/AgentFactory.cpp" 
    "Visualizer/Visualizer.h" 
    "Visualizer/Visualizer.cpp" 
    "GameAgent.h" 
    "Networks/Parameters.h" 
    "Storage/UtilityStorage.h" 
    "Storage/UtilityStorage.cpp"
)

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET Agent
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:Agent>)
endif (MSVC)

# Link libraries
target_link_libraries(Agent "${TORCH_LIBRARIES}")

# Set properties
set_property(TARGET Agent PROPERTY CXX_STANDARD 17)
