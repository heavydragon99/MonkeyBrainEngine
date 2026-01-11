
cmake_minimum_required(VERSION 3.20)

project(MonkeyBrainEngine
  VERSION 0.1.0
  LANGUAGES CXX
)

# Use latest stable C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build as a library (STATIC by default; change to SHARED if desired)
add_library(MonkeyBrainEngine)

# Add source files
file(GLOB_RECURSE MONKEYBRAINENGINE_SOURCES
  CONFIGURE_DEPENDS
  ${PROJECT_SOURCE_DIR}/code/src/*.cpp
)

target_sources(MonkeyBrainEngine
  PRIVATE
    ${MONKEYBRAINENGINE_SOURCES}
)

# Public include directory
target_include_directories(MonkeyBrainEngine
  PUBLIC
    ${PROJECT_SOURCE_DIR}/code/inc
)

# Reasonable warning flags
target_compile_options(MonkeyBrainEngine
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Ensure compile_commands.json is generated
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(game)
