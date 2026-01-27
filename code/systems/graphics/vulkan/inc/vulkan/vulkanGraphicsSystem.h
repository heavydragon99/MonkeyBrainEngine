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
  bool initialize(int aWidth, int aHeight) override;
  void shutdown() override;

  void update(Registry &, float) override;
  bool shouldQuit() const override;

  void drawDebug(const std::vector<DebugLine> &aLines) override;

private:
  void initSDL(int aWidth, int aHeight);
  void initVulkan();
  void pollEvents();
  void drawFrame();

  void createDebugPipeline();
  void createDebugVertexBuffer();
  void uploadDebugVertices(const std::vector<DebugLine> &aLines);

  vector2 toNDC(const vector2 &p) const;

private:
  SDL_Window *mWindow = nullptr;
  bool mQuit = false;

  VkInstance mInstance = VK_NULL_HANDLE;
  VkSurfaceKHR mSurface = VK_NULL_HANDLE;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mDevice = VK_NULL_HANDLE;

  uint32_t mGraphicsQueueFamily = UINT32_MAX;
  uint32_t mPresentQueueFamily = UINT32_MAX;

  VkQueue mGraphicsQueue = VK_NULL_HANDLE;
  VkQueue mPresentQueue = VK_NULL_HANDLE;

  VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
  VkFormat mSwapchainFormat{};
  VkExtent2D mSwapchainExtent{};

  std::vector<VkImage> mSwapchainImages;
  std::vector<VkImageView> mSwapchainImageViews;
  std::vector<VkFramebuffer> mFramebuffers;

  VkRenderPass mRenderPass = VK_NULL_HANDLE;
  VkCommandPool mCommandPool = VK_NULL_HANDLE;
  VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;

  VkSemaphore mImageAvailableSemaphore = VK_NULL_HANDLE;
  VkSemaphore mRenderFinishedSemaphore = VK_NULL_HANDLE;
  VkFence mInFlightFence = VK_NULL_HANDLE;

  // --- Debug rendering ---
  VkPipelineLayout mDebugPipelineLayout = VK_NULL_HANDLE;
  VkPipeline mDebugPipeline = VK_NULL_HANDLE;

  VkBuffer mDebugVertexBuffer = VK_NULL_HANDLE;
  VkDeviceMemory mDebugVertexMemory = VK_NULL_HANDLE;
  uint32_t mDebugVertexCount = 0;
};

#endif // VULKANGRAPHICSSYSTEM_H
