# --------------------------------------- #
#               CMAKE CONFIG              #
# --------------------------------------- #

# Define minimum CMake version required
cmake_minimum_required(VERSION 3.30)

# Define Project Name
project(LuminousLogger
        VERSION 1.0.0
        LANGUAGES CXX)

# Create global variable (project-level) to store output directory
set(LL_BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")

# Log directory at configuration-time
message("[${CMAKE_PROJECT_NAME}]: Output directory: ${LL_BIN_OUTPUT_DIR}")

# Set output directory for compiled libs and exe (project-level)
# MUST BE CALLED BEFORE `add_executables(...)` & `add_library(...)`
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LL_BIN_OUTPUT_DIR}/Libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LL_BIN_OUTPUT_DIR}/Libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${LL_BIN_OUTPUT_DIR}")

option(BUILD_DEBUG_MODE "Build in debug mode" ON)
option(BUILD_SHARED_LIB "Build shared library" OFF)
option(BUILD_STATIC_LIB "Build static library" OFF)
option(BUILD_MODULE_LIB "Build module library" OFF)



# --------------------------------------- #
#        AUTO-GENERATE SOURCES FILE       #
# --------------------------------------- #

file(GLOB_RECURSE LL_SOURCES "main.cpp" "Code/Src/*.cpp")

# --------------------------------------- #
#         SOURCE & HEADER INCLUDES        #
# --------------------------------------- #

# Defines how to build project
if(BUILD_DEBUG_MODE)
    add_executable(${PROJECT_NAME} ${LL_SOURCES})
endif()

if(BUILD_SHARED_LIB)
    add_library(${PROJECT_NAME} SHARED ${LL_SOURCES})
endif()

if(BUILD_STATIC_LIB)
    add_library(${PROJECT_NAME} STATIC ${LL_SOURCES})
endif()

if(BUILD_MODULE_LIB)
    add_library(${PROJECT_NAME} MODULE ${LL_SOURCES})
endif()


# Add Include Directory
target_include_directories(${PROJECT_NAME} PUBLIC "Code/Include")

# Define C++ Standard Version
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Set Warning level 4 and warnings as errors flags
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()