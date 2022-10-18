# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)

project("GameAgent" VERSION 0.1.0.0)
include(GNUInstallDirs)

# Set where libraries and executatbles are placed during build.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

# Used to build shared libraries.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Build position independent code (PIC).
# Useful for memory management with multiple shared libraries.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Disable C and C++ compiler extensions.
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup YCM (Files in cmake folder)
# Source: https://github.com/robotology/ycm/
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Create option for the user to select a Shared/Dynamic Library or a Static Library
option(BUILD_SHARED_LIBS "Build libraries as shared" ON)

# Build test related commands.
option(BUILD_TESTING "Create tests using CMake" OFF)
if(BUILD_TESTING)
    enable_testing()
endif()

# Enable RPATH support for installed binaries and libraries.
# TODO: Useful for managing multiple shared libraries in Linux and MacOS,
#       however project currently only support's Windows and MSVC. (Out-of-Scope)
include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_FULL_BINDIR}"
                          LIB_DIRS "${CMAKE_INSTALL_FULL_LIBDIR}"
                          INSTALL_NAME_DIR "${CMAKE_INSTALL_FULL_LIBDIR}"
                          USE_LINK_PATH)

# Specify a build type, otherwise set it to release.
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
    endif()
endif()

# Source code.
add_subdirectory("src")

# Add uninstall target
include(AddUninstallTarget)

# Add integration tests.
if (BUILD_TESTING)
    add_subdirectory("test")
endif()
