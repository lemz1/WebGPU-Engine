#include "surface.h"

namespace engine::wgpu
{
Surface::Surface(const Instance& instance)
{
  WGPUSurfaceDescriptor descriptor{};
  _handle = wgpuInstanceCreateSurface(instance, &descriptor);
}

Surface::~Surface()
{
  wgpuSurfaceRelease(_handle);
}
}  // namespace engine::wgpu
