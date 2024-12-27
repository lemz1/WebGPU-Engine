#include "instance.h"

namespace engine::wgpu
{
Instance::Instance()
{
  WGPUInstanceDescriptor descriptor{};
  descriptor.features.timedWaitAnyEnable = true;
  _handle = wgpuCreateInstance(&descriptor);
}

Instance::~Instance()
{
  wgpuInstanceRelease(_handle);
}
}  // namespace engine::wgpu
