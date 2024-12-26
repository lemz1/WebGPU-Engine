#include <webgpu/webgpu.h>

#include <iostream>

int main()
{
  WGPUInstanceDescriptor instanceDescriptor{};
  instanceDescriptor.features.timedWaitAnyEnable = true;
  WGPUInstance instance = wgpuCreateInstance(&instanceDescriptor);
  if (instance == nullptr)
  {
    std::cerr << "[WebGPU] Failed to create Instance!" << std::endl;
    return 1;
  }
  // Synchronously request the adapter.
  WGPURequestAdapterOptions options = {};
  WGPUAdapter adapter;
  WGPURequestAdapterCallback callback =
      [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
         WGPUStringView message, void* userdata)
  {
    if (status != WGPURequestAdapterStatus_Success)
    {
      std::cerr << "[WebGPU] Failed to get an adapter:" << message.data;
      return;
    }
    *static_cast<WGPUAdapter*>(userdata) = adapter;
  };
  wgpuInstanceRequestAdapter(instance, &options, callback, &adapter);
  if (adapter == nullptr)
  {
    std::cerr << "[WebGPU] Failed to request adapter!" << std::endl;
    return 1;
  }

  WGPUAdapterInfo info{};
  wgpuAdapterGetInfo(adapter, &info);

  std::cout << "Vendor ID: " << info.vendorID << std::endl;

  return 0;
}
