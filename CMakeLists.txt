cmake_minimum_required(VERSION 3.20)

project(MonkeyBrainEngine
  VERSION 0.1.0
  LANGUAGES CXX
)

# Use latest stable C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Build as a library (STATIC by default; change to SHARED if desired)
add_library(MonkeyBrainEngine)

# ---- CORE ENGINE SOURCES ----
# Top-level engine sources
file(GLOB_RECURSE ENGINE_CORE_SOURCES
    CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/code/src/*.cpp
)

target_sources(MonkeyBrainEngine
    PRIVATE
        ${ENGINE_CORE_SOURCES}
)

# Top-level include directory
target_include_directories(MonkeyBrainEngine
    PUBLIC
        ${PROJECT_SOURCE_DIR}/code/inc
)

# System.h include directory
target_include_directories(MonkeyBrainEngine
    PUBLIC
        ${PROJECT_SOURCE_DIR}/code/systems/inc
)

# ---- SYSTEMS ----
# Automatically add each system as a module
# Each system has its own inc/src folder
file(GLOB SYSTEM_FOLDERS RELATIVE ${PROJECT_SOURCE_DIR}/code/systems
     ${PROJECT_SOURCE_DIR}/code/systems/*)

foreach(SYSTEM_NAME ${SYSTEM_FOLDERS})
  if(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/code/systems/${SYSTEM_NAME}")
    # Grab sources inside system/src
    file(GLOB_RECURSE SYSTEM_SOURCES
            CONFIGURE_DEPENDS
            ${PROJECT_SOURCE_DIR}/code/systems/${SYSTEM_NAME}/src/*.cpp
        )

    # Add sources to engine library
    target_sources(MonkeyBrainEngine
            PRIVATE
                ${SYSTEM_SOURCES}
        )

    # Add system's inc folder as include directory
    target_include_directories(MonkeyBrainEngine
            PUBLIC
                ${PROJECT_SOURCE_DIR}/code/systems/${SYSTEM_NAME}/inc
        )
  endif()
endforeach()

# ---- WARNING FLAGS ----
target_compile_options(MonkeyBrainEngine
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

# Ensure compile_commands.json is generated
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- GAME SUBDIRECTORY ----
add_subdirectory(game)

