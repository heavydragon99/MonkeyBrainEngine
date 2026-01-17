#include "vulkan/vulkanGraphicsSystem.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

VulkanGraphicsSystem::~VulkanGraphicsSystem() { shutdown(); }

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
  drawFrame();

  // Rendering will go here later
  // For now: no-op
}

bool VulkanGraphicsSystem::shouldQuit() const { return mQuit; }

void VulkanGraphicsSystem::shutdown() {
  if (mDevice != VK_NULL_HANDLE) {
    // Make sure the GPU is completely idle before destroying anything.
    // Without this, the GPU could still be using resources you free.
    vkDeviceWaitIdle(mDevice);
  }

  /* -----------------------------------------------------------
     Synchronization objects
     ----------------------------------------------------------- */
  if (mInFlightFence != VK_NULL_HANDLE)
    vkDestroyFence(mDevice, mInFlightFence, nullptr);

  if (mRenderFinishedSemaphore != VK_NULL_HANDLE)
    vkDestroySemaphore(mDevice, mRenderFinishedSemaphore, nullptr);

  if (mImageAvailableSemaphore != VK_NULL_HANDLE)
    vkDestroySemaphore(mDevice, mImageAvailableSemaphore, nullptr);

  /* -----------------------------------------------------------
     Command buffers & pool
     ----------------------------------------------------------- */
  if (mCommandPool != VK_NULL_HANDLE)
    vkDestroyCommandPool(mDevice, mCommandPool, nullptr);
  // Command buffers are implicitly freed when the pool is destroyed

  /* -----------------------------------------------------------
     Framebuffers
     ----------------------------------------------------------- */
  for (VkFramebuffer fb : mFramebuffers)
    vkDestroyFramebuffer(mDevice, fb, nullptr);
  mFramebuffers.clear();

  /* -----------------------------------------------------------
     Render pass
     ----------------------------------------------------------- */
  if (mRenderPass != VK_NULL_HANDLE)
    vkDestroyRenderPass(mDevice, mRenderPass, nullptr);

  /* -----------------------------------------------------------
     Image views
     ----------------------------------------------------------- */
  for (VkImageView view : mSwapchainImageViews)
    vkDestroyImageView(mDevice, view, nullptr);
  mSwapchainImageViews.clear();

  /* -----------------------------------------------------------
     Swapchain
     ----------------------------------------------------------- */
  if (mSwapchain != VK_NULL_HANDLE)
    vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);

  /* -----------------------------------------------------------
     Logical device
     ----------------------------------------------------------- */
  if (mDevice != VK_NULL_HANDLE)
    vkDestroyDevice(mDevice, nullptr);

  /* -----------------------------------------------------------
     Surface (depends on instance)
     ----------------------------------------------------------- */
  if (mSurface != VK_NULL_HANDLE)
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);

  /* -----------------------------------------------------------
     Instance
     ----------------------------------------------------------- */
  if (mInstance != VK_NULL_HANDLE)
    vkDestroyInstance(mInstance, nullptr);

  /* -----------------------------------------------------------
     SDL
     ----------------------------------------------------------- */
  if (mWindow)
    SDL_DestroyWindow(mWindow);

  SDL_Quit();

  // Reset handles (good hygiene, helps debugging)
  mDevice = VK_NULL_HANDLE;
  mInstance = VK_NULL_HANDLE;
}

void VulkanGraphicsSystem::initSDL(int width, int height) {
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw std::runtime_error(std::string("Failed to init SDL: ") +
                             SDL_GetError());
  mWindow = SDL_CreateWindow("Vulkan Engine", width, height, SDL_WINDOW_VULKAN);
  if (mWindow == NULL) {
    throw std::runtime_error(std::string("Could not create window: ") +
                             SDL_GetError());
  }
}

void VulkanGraphicsSystem::initVulkan() {
  /* -----------------------------------------------------------
     1) INSTANCE (you already had this)
     ----------------------------------------------------------- */

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

  if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS)
    throw std::runtime_error("Failed to create Vulkan instance");

  /* -----------------------------------------------------------
     2) SURFACE (you already had this)
     ----------------------------------------------------------- */

  if (!SDL_Vulkan_CreateSurface(mWindow, mInstance, nullptr, &mSurface))
    throw std::runtime_error("Failed to create Vulkan surface");

  /* -----------------------------------------------------------
     3) PICK PHYSICAL DEVICE
     We need a GPU that supports:
     - Graphics
     - Presentation to our surface
     ----------------------------------------------------------- */

  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);
  if (deviceCount == 0)
    throw std::runtime_error("No Vulkan devices found");

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

  for (auto device : devices) {
    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);
    std::vector<VkQueueFamilyProperties> queues(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount,
                                             queues.data());

    for (uint32_t i = 0; i < queueCount; i++) {
      if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        mGraphicsQueueFamily = i;

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface,
                                           &presentSupport);

      if (presentSupport)
        mPresentQueueFamily = i;
    }

    if (mGraphicsQueueFamily != UINT32_MAX &&
        mPresentQueueFamily != UINT32_MAX) {
      mPhysicalDevice = device;
      break;
    }
  }

  if (mPhysicalDevice == VK_NULL_HANDLE)
    throw std::runtime_error("No suitable GPU found");

  /* -----------------------------------------------------------
     4) LOGICAL DEVICE + QUEUES
     ----------------------------------------------------------- */

  float priority = 1.0f;
  std::vector<VkDeviceQueueCreateInfo> queueInfos;

  std::vector<uint32_t> uniqueQueues = {mGraphicsQueueFamily,
                                        mPresentQueueFamily};

  for (uint32_t q : uniqueQueues) {
    VkDeviceQueueCreateInfo qInfo{};
    qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qInfo.queueFamilyIndex = q;
    qInfo.queueCount = 1;
    qInfo.pQueuePriorities = &priority;
    queueInfos.push_back(qInfo);
  }

  const char *deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  VkDeviceCreateInfo deviceInfo{};
  deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
  deviceInfo.pQueueCreateInfos = queueInfos.data();
  deviceInfo.enabledExtensionCount = 1;
  deviceInfo.ppEnabledExtensionNames = deviceExtensions;

  if (vkCreateDevice(mPhysicalDevice, &deviceInfo, nullptr, &mDevice) !=
      VK_SUCCESS)
    throw std::runtime_error("Failed to create logical device");

  vkGetDeviceQueue(mDevice, mGraphicsQueueFamily, 0, &mGraphicsQueue);
  vkGetDeviceQueue(mDevice, mPresentQueueFamily, 0, &mPresentQueue);

  /* -----------------------------------------------------------
     5) SWAPCHAIN ()
     ----------------------------------------------------------- */

  // --- Surface capabilities ---
  VkSurfaceCapabilitiesKHR caps{};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice, mSurface, &caps);

  // --- Surface formats ---
  uint32_t formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount,
                                       nullptr);

  if (formatCount == 0)
    throw std::runtime_error("No surface formats available");

  std::vector<VkSurfaceFormatKHR> formats(formatCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount,
                                       formats.data());

  // Prefer BGRA8 + SRGB, fallback to first
  VkSurfaceFormatKHR chosenFormat = formats[0];
  for (const auto &f : formats) {
    if (f.format == VK_FORMAT_B8G8R8A8_UNORM &&
        f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      chosenFormat = f;
      break;
    }
  }

  mSwapchainFormat = chosenFormat.format;

  // --- Swapchain extent ---
  if (caps.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    mSwapchainExtent = caps.currentExtent;
  } else {
    int w, h;
    SDL_GetWindowSize(mWindow, &w, &h);

    mSwapchainExtent.width =
        std::clamp(static_cast<uint32_t>(w), caps.minImageExtent.width,
                   caps.maxImageExtent.width);

    mSwapchainExtent.height =
        std::clamp(static_cast<uint32_t>(h), caps.minImageExtent.height,
                   caps.maxImageExtent.height);
  }

  // --- Image count (clamped) ---
  uint32_t imageCount = caps.minImageCount + 1;
  if (caps.maxImageCount > 0 && imageCount > caps.maxImageCount)
    imageCount = caps.maxImageCount;

  // --- Sharing mode ---
  VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  uint32_t queueFamilyIndices[] = {mGraphicsQueueFamily, mPresentQueueFamily};
  uint32_t queueFamilyIndexCount = 0;
  const uint32_t *pQueueFamilyIndices = nullptr;

  if (mGraphicsQueueFamily != mPresentQueueFamily) {
    sharingMode = VK_SHARING_MODE_CONCURRENT;
    queueFamilyIndexCount = 2;
    pQueueFamilyIndices = queueFamilyIndices;
  }

  // --- Create swapchain ---
  VkSwapchainCreateInfoKHR swapInfo{};
  swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapInfo.surface = mSurface;
  swapInfo.minImageCount = imageCount;
  swapInfo.imageFormat = chosenFormat.format;
  swapInfo.imageColorSpace = chosenFormat.colorSpace;
  swapInfo.imageExtent = mSwapchainExtent;
  swapInfo.imageArrayLayers = 1;
  swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapInfo.imageSharingMode = sharingMode;
  swapInfo.queueFamilyIndexCount = queueFamilyIndexCount;
  swapInfo.pQueueFamilyIndices = pQueueFamilyIndices;
  swapInfo.preTransform = caps.currentTransform;
  swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // always supported
  swapInfo.clipped = VK_TRUE;
  swapInfo.oldSwapchain = VK_NULL_HANDLE;

  VkResult res = vkCreateSwapchainKHR(mDevice, &swapInfo, nullptr, &mSwapchain);
  if (res != VK_SUCCESS)
    throw std::runtime_error("Failed to create swapchain");

  // --- Retrieve images ---
  vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, nullptr);
  mSwapchainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount,
                          mSwapchainImages.data());

  /* -----------------------------------------------------------
     6) IMAGE VIEWS
     ----------------------------------------------------------- */

  mSwapchainImageViews.resize(imageCount);
  for (size_t i = 0; i < imageCount; i++) {
    VkImageViewCreateInfo view{};
    view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view.image = mSwapchainImages[i];
    view.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view.format = mSwapchainFormat;
    view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view.subresourceRange.levelCount = 1;
    view.subresourceRange.layerCount = 1;

    vkCreateImageView(mDevice, &view, nullptr, &mSwapchainImageViews[i]);
  }

  /* -----------------------------------------------------------
     7) RENDER PASS (clear only)
     ----------------------------------------------------------- */

  VkAttachmentDescription color{};
  color.format = mSwapchainFormat;
  color.samples = VK_SAMPLE_COUNT_1_BIT;
  color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorRef{};
  colorRef.attachment = 0;
  colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorRef;

  VkRenderPassCreateInfo rpInfo{};
  rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  rpInfo.attachmentCount = 1;
  rpInfo.pAttachments = &color;
  rpInfo.subpassCount = 1;
  rpInfo.pSubpasses = &subpass;

  vkCreateRenderPass(mDevice, &rpInfo, nullptr, &mRenderPass);

  /* -----------------------------------------------------------
     8) FRAMEBUFFERS
     ----------------------------------------------------------- */

  mFramebuffers.resize(imageCount);
  for (size_t i = 0; i < imageCount; i++) {
    VkImageView attachments[] = {mSwapchainImageViews[i]};

    VkFramebufferCreateInfo fb{};
    fb.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb.renderPass = mRenderPass;
    fb.attachmentCount = 1;
    fb.pAttachments = attachments;
    fb.width = mSwapchainExtent.width;
    fb.height = mSwapchainExtent.height;
    fb.layers = 1;

    vkCreateFramebuffer(mDevice, &fb, nullptr, &mFramebuffers[i]);
  }

  /* -----------------------------------------------------------
     9) COMMAND POOL + BUFFER
     ----------------------------------------------------------- */

  VkCommandPoolCreateInfo pool{};
  pool.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool.queueFamilyIndex = mGraphicsQueueFamily;
  pool.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  vkCreateCommandPool(mDevice, &pool, nullptr, &mCommandPool);

  VkCommandBufferAllocateInfo alloc{};
  alloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc.commandPool = mCommandPool;
  alloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc.commandBufferCount = 1;

  vkAllocateCommandBuffers(mDevice, &alloc, &mCommandBuffer);

  /* -----------------------------------------------------------
     10) SYNCHRONIZATION
     ----------------------------------------------------------- */

  VkSemaphoreCreateInfo sem{};
  sem.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  vkCreateSemaphore(mDevice, &sem, nullptr, &mImageAvailableSemaphore);
  vkCreateSemaphore(mDevice, &sem, nullptr, &mRenderFinishedSemaphore);

  VkFenceCreateInfo fence{};
  fence.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  vkCreateFence(mDevice, &fence, nullptr, &mInFlightFence);

  std::cout << "Vulkan initialized successfully.\n";
  std::cout << "For future add validation layers!\n";
  std::cout << "For future fix resize handling!\n";
  // TODO: Add validation layers
  // TODO: Handle window resizing
}

void VulkanGraphicsSystem::pollEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT)
      mQuit = true;
  }
}

void VulkanGraphicsSystem::drawFrame() {
  // Wait until the GPU finished the previous frame
  vkWaitForFences(mDevice, 1, &mInFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(mDevice, 1, &mInFlightFence);

  // Acquire an image from the swapchain
  uint32_t imageIndex;
  vkAcquireNextImageKHR(mDevice, mSwapchain, UINT64_MAX,
                        mImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

  // Reset command buffer for reuse
  vkResetCommandBuffer(mCommandBuffer, 0);

  // Begin recording commands
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  vkBeginCommandBuffer(mCommandBuffer, &beginInfo);

  // Clear the screen to dark gray
  VkClearValue clearColor{};
  clearColor.color = {{0.1f, 0.1f, 0.1f, 1.0f}};

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = mRenderPass;
  renderPassInfo.framebuffer = mFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = mSwapchainExtent;
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  // Begin render pass (no drawing inside)
  vkCmdBeginRenderPass(mCommandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  vkCmdEndRenderPass(mCommandBuffer);
  vkEndCommandBuffer(mCommandBuffer);

  // Submit command buffer
  VkPipelineStageFlags waitStage =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = &mImageAvailableSemaphore;
  submitInfo.pWaitDstStageMask = &waitStage;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &mCommandBuffer;
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = &mRenderFinishedSemaphore;

  vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, mInFlightFence);

  // Present the image to the window
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &mRenderFinishedSemaphore;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &mSwapchain;
  presentInfo.pImageIndices = &imageIndex;

  vkQueuePresentKHR(mPresentQueue, &presentInfo);
}
