#include "adapter.h"

#include <iostream>

#include "engine/wgpu/util.h"

namespace engine::wgpu
{
static void RequestAdapater(WGPURequestAdapterStatus status,
                            WGPUAdapter adapter, WGPUStringView message,
                            void* userdata);

Adapter::Adapter(const Instance& instance)
{
  WGPURequestAdapterOptions options = {};
  wgpuInstanceRequestAdapter(instance, &options, RequestAdapater, &_handle);
  if (!_handle)
  {
    std::cerr << "[WebGPU] Failed to request adapter!" << std::endl;
  }
}

Adapter::~Adapter()
{
  wgpuAdapterRelease(_handle);
}

static void RequestAdapater(WGPURequestAdapterStatus status,
                            WGPUAdapter adapter, WGPUStringView message,
                            void* userdata)
{
  if (status != WGPURequestAdapterStatus_Success)
  {
    std::cerr << "[WebGPU] Failed to get an adapter:" << WGPUString(message)
              << std::endl;
    return;
  }
  *static_cast<WGPUAdapter*>(userdata) = adapter;
}

}  // namespace engine::wgpu
