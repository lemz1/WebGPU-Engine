#include "surface.h"

#include <glfw3webgpu.h>

namespace engine::wgpu
{
Surface::Surface(const Instance& instance, const engine::core::Window& window)
{
  _handle = glfwGetWGPUSurface(instance, window);
}

Surface::~Surface()
{
  wgpuSurfaceRelease(_handle);
}
}  // namespace engine::wgpu
