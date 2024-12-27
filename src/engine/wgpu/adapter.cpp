#include "adapter.h"

#include <iostream>

namespace engine::wgpu
{
static void RequestAdapater(WGPURequestAdapterStatus status,
                            WGPUAdapter adapter, WGPUStringView message,
                            void* userdata);

Adapter::Adapter(const Instance& instance)
{
  WGPURequestAdapterOptions options = {};
  WGPURequestAdapterCallback callback = RequestAdapater;
  wgpuInstanceRequestAdapter(instance, &options, callback, &_handle);
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
    std::cerr << "[WebGPU] Failed to get an adapter:" << message.data;
    return;
  }
  *static_cast<WGPUAdapter*>(userdata) = adapter;
}

}  // namespace engine::wgpu
