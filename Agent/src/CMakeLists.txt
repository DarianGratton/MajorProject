# CMakeList.txt : CMake project for Agent, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.14)

find_package(Torch REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Setup include directory
set(INCLUDE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/include")
message(STATUS "Using include directory: ${INCLUDE_DIR}")
include_directories(${INCLUDE_DIR})

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

### Build Library
set(LIBRARY_TARGET_NAME "${PROJECT_NAME}")

# List of CPP (source) library files.
set(${LIBRARY_TARGET_NAME}_SRC
    "Agent.cpp"
    "Environment/Environment.cpp"
    "Environment/State.cpp"
    "Networks/ACER/ReplayMemory.cpp" 
    "Networks/ACER/ActorCriticNetwork.cpp" 
    "Networks/ACER/ACERAgent.cpp" 
    "Networks/AgentFactory.cpp" 
    "Visualizer/Visualizer.cpp" 
    "Storage/UtilityStorage.cpp"
)

# List of HPP (header) library files
set(${LIBRARY_TARGET_NAME}_HDR
    "Agent.h" 
    "Environment/Environment.h"
    "Environment/State.h" 
    "Networks/ACER/RandomIterator.h"  
    "Networks/ACER/ReplayMemory.h" 
    "Networks/ACER/ActorCriticNetwork.h" 
    "Networks/ACER/Trajectory.h" 
    "Networks/NetworkAgent.h" 
    "Networks/NetworkParameters.h" 
    "Networks/ACER/ACERParameters.h" 
    "Networks/AgentFactory.h" 
    "Visualizer/Visualizer.h" 
    "GameAgent.h" 
    "Networks/Parameters.h" 
    "Storage/UtilityStorage.h" 
)

# Add a library to be built from the source and header files.
add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})

# Create alias
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

# Set two minumum target properties for the library.
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION       "${${PROJECT_NAME}_VERSION}"
                                                        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

# Specify include directories for both comilation and installation process
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                         "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# Link required libraries
target_link_libraries(${LIBRARY_TARGET_NAME} "${TORCH_LIBRARIES}")

# Specify installation targets, typology and destination folders.
install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev)

message(STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
