# CMakeList.txt : CMake project for Agent, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package(Torch REQUIRED)

# Add source to this project's executable.
add_executable (Agent "Main.cpp" "Agent.cpp" "Agent.h" "Environment/Environment.h" "Environment/Environment.cpp" "Environment/Reward.h" "Environment/Reward.cpp" "Environment/State.h" "Environment/State.cpp" "Networks/SAC/ReplayMemory.h" "Networks/SAC/SACAgent.h" "Networks/SAC/CriticNetwork.h" "Networks/SAC/ValueNetwork.h" "Networks/SAC/PolicyNetwork.h" "Networks/SAC/CriticNetwork.cpp" "Networks/SAC/PolicyNetwork.cpp" "Networks/SAC/ValueNetwork.cpp" "Networks/SAC/SACAgent.cpp" "Networks/SAC/ReplayMemory.cpp")

# Link libraries
target_link_libraries(Agent "${TORCH_LIBRARIES}")

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
