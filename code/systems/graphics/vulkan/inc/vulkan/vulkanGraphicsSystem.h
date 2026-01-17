#ifndef VULKANGRAPHICSSYSTEM_H
#define VULKANGRAPHICSSYSTEM_H

#include "graphics/graphicsSystem.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanGraphicsSystem final : public GraphicsSystem {
public:
  ~VulkanGraphicsSystem() override;
  bool initialize() override;
  bool initialize(int width, int height) override;
  void shutdown() override;

  void update(Registry &registry, float dt) override;

  bool shouldQuit() const override;

private:
  void initSDL(int width, int height);
  void initVulkan();
  void pollEvents();
  void drawFrame();

private:
  SDL_Window *mWindow = nullptr;
  bool mQuit = false;

  VkInstance mInstance = VK_NULL_HANDLE;
  VkSurfaceKHR mSurface = VK_NULL_HANDLE;

  // --- Vulkan device ---
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mDevice = VK_NULL_HANDLE;

  // --- Queues ---
  VkQueue mGraphicsQueue = VK_NULL_HANDLE;
  VkQueue mPresentQueue = VK_NULL_HANDLE;
  uint32_t mGraphicsQueueFamily = 0;
  uint32_t mPresentQueueFamily = 0;

  // --- Swapchain ---
  VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
  VkFormat mSwapchainFormat;
  VkExtent2D mSwapchainExtent;
  std::vector<VkImage> mSwapchainImages;
  std::vector<VkImageView> mSwapchainImageViews;

  // --- Rendering ---
  VkRenderPass mRenderPass = VK_NULL_HANDLE;
  std::vector<VkFramebuffer> mFramebuffers;

  // --- Commands ---
  VkCommandPool mCommandPool = VK_NULL_HANDLE;
  VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;

  // --- Sync ---
  VkSemaphore mImageAvailableSemaphore = VK_NULL_HANDLE;
  VkSemaphore mRenderFinishedSemaphore = VK_NULL_HANDLE;
  VkFence mInFlightFence = VK_NULL_HANDLE;
};
#endif // VULKANGRAPHICSSYSTEM_H
