# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.15)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  ModernCMakeExample
  VERSION 0.1
  LANGUAGES CXX)

# If you set any CMAKE_ variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)

# Find packages go here.

# Link each target with other targets or add options, etc.

<<<<<<< HEAD
<<<<<<< HEAD
include_directories(include)

#target_sources()

# Adding something we can run - Output name matches target name
add_executable(chunked_ecs_poc
  main.cpp
  src/Archetype.cpp
  src/ECS.cpp)
=======
include_directories(engine)

# Adding something we can run - Output name matches target name
add_executable(chunked_poc game/main.cpp)
>>>>>>> 93a0220 (Added poc docs and poc)
=======
include_directories(include)

#target_sources()

# Adding something we can run - Output name matches target name
add_executable(chunked_ecs_poc
  main.cpp
  src/Archetype.cpp
  src/ECS.cpp)
>>>>>>> b64bd00 (Updated chunked ECS poc)
