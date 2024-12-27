#include "device.h"

#include <iostream>

namespace engine::wgpu
{
static void RequestDevice(WGPURequestDeviceStatus status, WGPUDevice device,
                          WGPUStringView message, void* userdata);

Device::Device(const Adapter& adapter, const WGPUDeviceDescriptor* descriptor)
{
  WGPURequestDeviceCallback callback = RequestDevice;
  wgpuAdapterRequestDevice(adapter, descriptor, callback, &_handle);
  if (!_handle)
  {
    std::cerr << "[WebGPU] Could not request Device" << std::endl;
  }
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
    std::cerr << "[WebGPU] Failed to get a Device";
    return;
  }
  *static_cast<WGPUDevice*>(userdata) = device;
}
}  // namespace engine::wgpu
