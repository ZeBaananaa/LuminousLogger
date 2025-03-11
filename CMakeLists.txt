# --------------------------------------- #
#               CMAKE CONFIG              #
# --------------------------------------- #

# Define minimum CMake version required
cmake_minimum_required(VERSION 3.20)

# Define Project Name
project(LuminousLogger
        VERSION 1.0.0
        LANGUAGES CXX)

# Define output build folder
set(LL_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Builds/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")

# Define Output Build path for libraries & executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LL_OUTPUT_DIR}/Libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LL_OUTPUT_DIR}/Libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${LL_OUTPUT_DIR}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


#---------------------------------------#
#                SOURCES                #
#---------------------------------------#
file(GLOB_RECURSE L_LOGGER_SOURCES "Code/*.cpp")


#---------------------------------------#
#            BUILD MODE                 #
#---------------------------------------#
add_library(L_LOGGER SHARED ${L_LOGGER_SOURCES})


#---------------------------------------#
#                INCLUDE                #
#---------------------------------------#
target_include_directories(L_LOGGER PUBLIC "Code/Include")


#---------------------------------------#
#                COMPILE MODE           #
#---------------------------------------#
target_compile_features(L_LOGGER PUBLIC cxx_std_20)

if (MSVC)
    target_compile_options(L_LOGGER PUBLIC /W3 /WX)
else ()
    target_compile_options(L_LOGGER PUBLIC -Wall -Wextra -Wpedantic)
endif ()

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Build position independent code.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Disable C and C++ compiler extensions.
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)


#---------------------------------------#
#        EXPORTS / IMPORTS FOR DLL      #
#---------------------------------------#
target_compile_definitions(L_LOGGER PRIVATE LUMINOUS_LOGGER_EXPORTS)

# Disable lib prefix
set_target_properties(L_LOGGER PROPERTIES PREFIX "")

# Set dll name
set_target_properties(L_LOGGER PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")



add_subdirectory(Test)