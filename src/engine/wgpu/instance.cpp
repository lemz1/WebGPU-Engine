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

void Instance::WaitAny(std::vector<WGPUFutureWaitInfo>& futures) const
{
  wgpuInstanceWaitAny(_handle, futures.size(), futures.data(), UINT64_MAX);
}
}  // namespace engine::wgpu
