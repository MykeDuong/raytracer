cmake_minimum_required(VERSION 3.10)

project(Raytracer VERSION 1.0)

set(CMAKE_CXX_STANDARD, 14)
set(CMAKE_CXX_STANDARD_REQUIRED, True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CXXFLAGS  "-Wall -Werror -g -O5")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

include_directories(include)
#include_directories(template)

configure_file(./include/version.h.in version.h)

file(GLOB SOURCES "src/*.cc")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc")
set(MAIN "src/main.cc")
message(SOURCES="${SOURCES}")
add_executable(raytracer ${SOURCES} ${MAIN})
target_compile_definitions(raytracer PUBLIC VERDANT_FLAG_DEBUG)
target_include_directories(raytracer PUBLIC "${PROJECT_BINARY_DIR}")
