#include "monkeyBrainEngine.h"

#include <SDL3/SDL_video.h>
#include <iostream>

//  int main() {
//   MonkeyBrainEngine engine;
//   Registry &registry = engine.getRegistry();
//
//   Entity player = registry.createEntity();
//   Entity enemy = registry.createEntity();
//   Entity npc = registry.createEntity();
//
//   registry.add(player, Transform{{0, 0}, 0, {1, 1}});
//   registry.add(player, Velocity{{1, 2}});
//
//   registry.add(enemy, Transform{{10, 10}, 0, {1, 1}});
//
//   registry.add(npc, Transform{{5, 5}, 0, {1, 1}});
//   try {
//     engine.initialize();
//   } catch (const std::exception &e) {
//     std::cerr << "Failed to initialize engine: " << e.what() << "\n";
//     return -1;
//   }
//   try {
//     engine.run();
//   } catch (const std::exception &e) {
//     std::cerr << "Error during engine run: " << e.what() << "\n";
//   }
//   engine.shutdown();
// }

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[]) {

  SDL_Window *window; // Declare a pointer
  bool done = false;

  SDL_Init(SDL_INIT_VIDEO); // Initialize SDL3

  // Create an application window with the following settings:
  window = SDL_CreateWindow("An SDL3 window", // window title
                            640,              // width, in pixels
                            480,              // height, in pixels
                            SDL_WINDOW_OPENGL // flags - see below
  );

  // Check that the window was successfully created
  if (window == NULL) {
    // In the case that the window could not be made...
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                 SDL_GetError());
    return 1;
  }

  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }

    // Do game logic, present a frame, etc.
  }

  // Close and destroy the window
  SDL_DestroyWindow(window);

  // Clean up
  SDL_Quit();
  return 0;
}
