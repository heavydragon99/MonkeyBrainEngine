#ifndef VULKANGRAPHICSSYSTEM_H
#define VULKANGRAPHICSSYSTEM_H

#include "graphics/graphicsSystem.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

class VulkanGraphicsSystem final : public GraphicsSystem {
public:
  bool initialize() override;
  bool initialize(int width, int height) override;
  void shutdown() override;

  void update(Registry &registry, float dt) override;

  bool shouldQuit() const override;

private:
  void initSDL(int width, int height);
  void initVulkan();
  void pollEvents();

private:
  SDL_Window *m_window = nullptr;
  bool m_quit = false;

  VkInstance m_instance = VK_NULL_HANDLE;
  VkSurfaceKHR m_surface = VK_NULL_HANDLE;
};
#endif // VULKANGRAPHICSSYSTEM_H
