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

# ---- GRAPHICS BACKEND SELECTION ----
set(GRAPHICS_BACKEND "linux_vulkan" CACHE STRING
    "Graphics backend (linux_vulkan, windows_dx12, mac_metal)"
)

set(VALID_GRAPHICS_BACKENDS
    linux_vulkan
    windows_dx12
    mac_metal
)

list(FIND VALID_GRAPHICS_BACKENDS ${GRAPHICS_BACKEND} BACKEND_INDEX)
if(BACKEND_INDEX EQUAL -1)
  message(FATAL_ERROR
        "Invalid GRAPHICS_BACKEND: ${GRAPHICS_BACKEND}\n"
        "Valid values: ${VALID_GRAPHICS_BACKENDS}"
    )
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

# ---- SYSTEMS (NON-GRAPHICS) ----
# Automatically add each system as a module
# Each system has its own inc/src folder
file(GLOB SYSTEM_FOLDERS RELATIVE ${PROJECT_SOURCE_DIR}/code/systems
     ${PROJECT_SOURCE_DIR}/code/systems/*)

foreach(SYSTEM_NAME ${SYSTEM_FOLDERS})
  if(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/code/systems/${SYSTEM_NAME}"
     AND NOT SYSTEM_NAME STREQUAL "graphics")
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

# ---- GRAPHICS SYSTEM INTERFACE ----
target_include_directories(MonkeyBrainEngine
    PUBLIC
        ${PROJECT_SOURCE_DIR}/code/systems/graphics/inc
)


# ---- GRAPHICS BACKEND IMPLEMENTATION ----
if(GRAPHICS_BACKEND STREQUAL "linux_vulkan")

  if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "linux_vulkan selected but platform is not Linux")
  endif()

  message(STATUS "Using graphics backend: Linux Vulkan")

  file(GLOB_RECURSE GRAPHICS_BACKEND_SOURCES
        CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/code/systems/graphics/vulkan/src/*.cpp
    )

  target_sources(MonkeyBrainEngine PRIVATE ${GRAPHICS_BACKEND_SOURCES})

  target_include_directories(MonkeyBrainEngine
        PUBLIC
            ${PROJECT_SOURCE_DIR}/code/systems/graphics/vulkan/inc
    )

  find_package(Vulkan REQUIRED)
  find_package(SDL3 REQUIRED)

  target_link_libraries(MonkeyBrainEngine
        PUBLIC
            Vulkan::Vulkan
            SDL3::SDL3
    )

  target_compile_definitions(MonkeyBrainEngine
        PUBLIC
            MB_PLATFORM_LINUX
            MB_GRAPHICS_VULKAN
    )

elseif(GRAPHICS_BACKEND STREQUAL "windows_dx12")

  message(FATAL_ERROR "windows_dx12 backend not implemented yet")

elseif(GRAPHICS_BACKEND STREQUAL "mac_metal")

  message(FATAL_ERROR "mac_metal backend not implemented yet")

endif()


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

