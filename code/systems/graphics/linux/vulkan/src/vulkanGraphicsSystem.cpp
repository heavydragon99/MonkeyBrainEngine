#include "vulkan/vulkanGraphicsSystem.h"

#include <iostream>
#include <stdexcept>
#include <vector>

bool VulkanGraphicsSystem::initialize() {
  // Default window size
  initialize(800, 600);
  std::cout << "Vulkan Graphics System initialized.\n";
  return true;
}

bool VulkanGraphicsSystem::initialize(int width, int height) {
  initSDL(width, height);
  initVulkan();
  return true;
}

void VulkanGraphicsSystem::update(Registry &, float) {
  pollEvents();

  // Rendering will go here later
  // For now: no-op
}

bool VulkanGraphicsSystem::shouldQuit() const { return m_quit; }

void VulkanGraphicsSystem::shutdown() {
  if (m_surface != VK_NULL_HANDLE)
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

  if (m_instance != VK_NULL_HANDLE)
    vkDestroyInstance(m_instance, nullptr);

  if (m_window)
    SDL_DestroyWindow(m_window);

  SDL_Quit();
}

void VulkanGraphicsSystem::initSDL(int width, int height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    throw std::runtime_error(std::string("Failed to init SDL: ") +
                             SDL_GetError());

  m_window =
      SDL_CreateWindow("Vulkan Engine", width, height, SDL_WINDOW_VULKAN);

  if (!m_window)
    throw std::runtime_error(std::string("Failed to create SDL window: ") +
                             SDL_GetError());
}

void VulkanGraphicsSystem::initVulkan() {
  // SDL3 Vulkan extensions
  Uint32 extensionCount = 0;
  const char *const *extensions =
      SDL_Vulkan_GetInstanceExtensions(&extensionCount);

  std::vector<const char *> extensionList(extensions,
                                          extensions + extensionCount);

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "My Engine";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "MonkeyBrainEngine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_2;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(extensionList.size());
  createInfo.ppEnabledExtensionNames = extensionList.data();

  if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    throw std::runtime_error("Failed to create Vulkan instance");

  // SDL3 Vulkan surface
  if (!SDL_Vulkan_CreateSurface(m_window, m_instance, nullptr, &m_surface))
    throw std::runtime_error("Failed to create Vulkan surface");
}

void VulkanGraphicsSystem::pollEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT)
      m_quit = true;
  }
}
