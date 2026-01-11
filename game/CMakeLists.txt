# Collect game source files
file(GLOB_RECURSE MONKEYBRAINGAME_SOURCES
  CONFIGURE_DEPENDS
  ${PROJECT_SOURCE_DIR}/game/code/src/*.cpp
)

# Create the game executable
add_executable(MonkeyBrainGame
  ${MONKEYBRAINGAME_SOURCES}
)

# Link against the engine library
target_link_libraries(MonkeyBrainGame
  PRIVATE
    MonkeyBrainEngine
)

# Use the same C++ standard as the engine
target_compile_features(MonkeyBrainGame
  PRIVATE
    cxx_std_23
)
