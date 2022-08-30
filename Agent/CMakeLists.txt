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

# To add a trailing "d" to library built in debug mode.
# NOTE: Unknown if needed as project can not be built in debug mode, it crashes.
if(MSVC)
	set(CMAKE_DEBUG_POSTFIX "d")
endif()

# Build position independent code (PIC).
# Useful for memory management with multiple shared libraries.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Disable C and C++ compiler extensions.
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup YCM
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Shared/Dynamic Library or Static Library?
option(BUILD_SHARED_LIBS "Build libraries as shared" ON)

# Build test related commands.
# NOTE: Unknown what this is or if it's needed for the scope of project.
option(BUILD_TESTING "Create tests using CMake" OFF)
if(BUILD_TESTING)
    enable_testing()
endif()

# Enable RPATH support for installed binaries and libraries.
# NOTE: Apparent useful for managing multiple shared libraries in Linux and MacOS,
#       unknown if useful for Windows and MSVC.
include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_FULL_BINDIR}"
                          LIB_DIRS "${CMAKE_INSTALL_FULL_LIBDIR}"
                          INSTALL_NAME_DIR "${CMAKE_INSTALL_FULL_LIBDIR}"
                          USE_LINK_PATH)

# Specify a build type, otherwise set it to release.
# NOTE: Again unknown if needed for the scope of this project.
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
    endif()
endif()

# Source code.
add_subdirectory("src")

# Create and install CMake configuration files (Needed for find_package()). 
# TODO: Return to and change configuration options. 
#       Requires DEPENDENCIES as library requires PyTorch.
#include(InstallBasicPackageFiles)
#install_basic_package_files(${PROJECT_NAME}
#                            VERSION ${${PROJECT_NAME}_VERSION}
#                            COMPATIBILITY AnyNewerVersion
#                            VARS_PREFIX ${PROJECT_NAME}
#                            NO_CHECK_REQUIRED_COMPONENTS_MACRO)

# Add uninstall target
include(AddUninstallTarget)

# Add integration tests.
if (BUILD_TESTING)
    add_subdirectory("test")
endif()
