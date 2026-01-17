#ifndef GRAPHICSBACKEND_H
#define GRAPHICSBACKEND_H

#if defined(MB_GRAPHICS_VULKAN)

#include "vulkan/vulkanGraphicsSystem.h"
using ActiveGraphicsSystem = VulkanGraphicsSystem;

#elif defined(MB_GRAPHICS_OPENGL)

#include "opengl/openglGraphicsSystem.h"
using ActiveGraphicsSystem = OpenGLGraphicsSystem;

#elif defined(MB_GRAPHICS_METAL)

#include "metal/metalGraphicsSystem.h"
using ActiveGraphicsSystem = MetalGraphicsSystem;

#elif defined(MB_GRAPHICS_DX12)

#include "dx12/dx12GraphicsSystem.h"
using ActiveGraphicsSystem = DX12GraphicsSystem;

#else
#error "No graphics backend selected. Check GRAPHICS_BACKEND CMake option."
#endif
#endif // GRAPHICSBACKEND_H
