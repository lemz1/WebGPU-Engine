#include "device.h"

#include <iostream>

#include "engine/wgpu/util.h"

namespace engine::wgpu
{
static void RequestDevice(WGPURequestDeviceStatus status, WGPUDevice device,
                          WGPUStringView message, void* userdata);

static void LogDevice(WGPULoggingType type, WGPUStringView message,
                      void* userdata);

Device::Device(const Adapter& adapter, const WGPUDeviceDescriptor* descriptor)
{
  wgpuAdapterRequestDevice(adapter, descriptor, RequestDevice, &_handle);
  if (!_handle)
  {
    std::cerr << "[WebGPU] Could not request Device" << std::endl;
  }

  wgpuDeviceSetLoggingCallback(_handle, LogDevice, nullptr);
}

Device::~Device()
{
  wgpuDeviceRelease(_handle);
}

static void RequestDevice(WGPURequestDeviceStatus status, WGPUDevice device,
                          WGPUStringView message, void* userdata)
{
  if (status != WGPURequestDeviceStatus_Success)
  {
    std::cerr << "[WebGPU] Failed to get a Device" << std::endl;
    return;
  }
  *static_cast<WGPUDevice*>(userdata) = device;
}

static void LogDevice(WGPULoggingType type, WGPUStringView message,
                      void* userdata)
{
  switch (type)
  {
    case WGPULoggingType_Error:
      std::cerr << "[WebGPU] Error: " << WGPUString(message) << std::endl;
      break;
    default:
      std::cout << "[WebGPU] Info: " << WGPUString(message) << std::endl;
      break;
  }
}
}  // namespace engine::wgpu
