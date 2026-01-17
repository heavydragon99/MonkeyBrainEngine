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
    throw std::runtime_error("Failed to init SDL");

  m_window = SDL_CreateWindow("Vulkan Engine", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);

  if (!m_window)
    throw std::runtime_error("Failed to create SDL window");
}

void VulkanGraphicsSystem::initVulkan() {
  unsigned int extensionCount = 0;
  SDL_Vulkan_GetInstanceExtensions(m_window, &extensionCount, nullptr);

  std::vector<const char *> extensions(extensionCount);
  SDL_Vulkan_GetInstanceExtensions(m_window, &extensionCount,
                                   extensions.data());

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
  createInfo.enabledExtensionCount = extensions.size();
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    throw std::runtime_error("Failed to create Vulkan instance");

  if (!SDL_Vulkan_CreateSurface(m_window, m_instance, &m_surface))
    throw std::runtime_error("Failed to create Vulkan surface");
}

void VulkanGraphicsSystem::pollEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)
      m_quit = true;
  }
}
