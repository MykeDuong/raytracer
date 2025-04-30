cmake_minimum_required(VERSION 3.10)

project(Raytracer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
file(GLOB SOURCES "src/*.cc")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc")
set(MAIN "src/main.cc")


# Generate version header
configure_file(./include/version.h.in version.h)

# Executable
add_executable(raytracer ${SOURCES} ${MAIN})

# Include directories
include_directories(include)
target_include_directories(raytracer PUBLIC "${PROJECT_BINARY_DIR}")

# Platform-specific compiler flags
target_compile_options(raytracer
  PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Werror -g -O3>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /O2 /Ob /Ob2 /favor:AMD64 /d2vzeroupper>
)

# Preprocessor definition
target_compile_definitions(raytracer PUBLIC VERDANT_FLAG_DEBUG)