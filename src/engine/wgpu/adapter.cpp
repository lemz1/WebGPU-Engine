#include "adapter.h"

#include <iostream>

#include "engine/wgpu/util.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace engine::wgpu
{
struct AdapterUserData
{
  bool finished = false;
  WGPUAdapter adapter = nullptr;
};

static void RequestAdapater(WGPURequestAdapterStatus status,
                            WGPUAdapter adapter, WGPUStringView message,
                            void* userdata);

Adapter::Adapter(const Instance& instance)
{
  AdapterUserData data;
  WGPURequestAdapterOptions options = {};
  wgpuInstanceRequestAdapter(instance, &options, RequestAdapater, &data);

#ifdef __EMSCRIPTEN__
  while (!data.finished)
  {
    emscripten_sleep(100);
  }
#endif

  if (!data.adapter)
  {
    std::cerr << "[WebGPU] Failed to request adapter!" << std::endl;
    return;
  }

  _handle = data.adapter;
}

Adapter::~Adapter()
{
  wgpuAdapterRelease(_handle);
}

static void RequestAdapater(WGPURequestAdapterStatus status,
                            WGPUAdapter adapter, WGPUStringView message,
                            void* userdata)
{
  auto& data = *static_cast<AdapterUserData*>(userdata);
  data.finished = true;
  if (status != WGPURequestAdapterStatus_Success)
  {
    std::cerr << "[WebGPU] Failed to get an adapter:" << WGPUToString(message)
              << std::endl;
    return;
  }
  data.adapter = adapter;
}

}  // namespace engine::wgpu
