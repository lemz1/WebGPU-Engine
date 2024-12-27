#include "device.h"

#include <iostream>

#include "engine/wgpu/util.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace engine::wgpu
{
struct DeviceUserData
{
  bool finished = false;
  WGPUDevice device = nullptr;
};

static void RequestDevice(WGPURequestDeviceStatus status, WGPUDevice device,
                          WGPUStringView message, void* userdata);

static void UncapturedError(WGPUDevice const* device, WGPUErrorType type,
                            struct WGPUStringView message, void* userdata1,
                            void* userdata2);

static void LostCallback(WGPUDevice const* device, WGPUDeviceLostReason reason,
                         struct WGPUStringView message, void* userdata1,
                         void* userdata2);

Device::Device(const Adapter& adapter, WGPUDeviceDescriptor descriptor)
{
  if (descriptor.uncapturedErrorCallbackInfo2.callback == nullptr)
  {
    descriptor.uncapturedErrorCallbackInfo2.callback = UncapturedError;
    descriptor.uncapturedErrorCallbackInfo2.nextInChain = nullptr;
    descriptor.uncapturedErrorCallbackInfo2.userdata1 = nullptr;
    descriptor.uncapturedErrorCallbackInfo2.userdata2 = nullptr;
  }

  if (descriptor.deviceLostCallbackInfo2.callback == nullptr)
  {
    descriptor.deviceLostCallbackInfo2.callback = LostCallback;
    descriptor.deviceLostCallbackInfo2.nextInChain = nullptr;
    descriptor.deviceLostCallbackInfo2.mode = WGPUCallbackMode_WaitAnyOnly;
    descriptor.deviceLostCallbackInfo2.userdata1 = nullptr;
    descriptor.deviceLostCallbackInfo2.userdata2 = nullptr;
  }

  DeviceUserData data{};
  wgpuAdapterRequestDevice(adapter, &descriptor, RequestDevice, &data);

#ifdef __EMSCRIPTEN__
  while (!data.finished)
  {
    emscripten_sleep(100);
  }
#endif

  if (!data.device)
  {
    std::cerr << "[WebGPU] Could not request Device" << std::endl;
    return;
  }
  _handle = data.device;
}

Device::~Device()
{
  wgpuDeviceRelease(_handle);
}

static void RequestDevice(WGPURequestDeviceStatus status, WGPUDevice device,
                          WGPUStringView message, void* userdata)
{
  auto& data = *static_cast<DeviceUserData*>(userdata);
  data.finished = true;
  if (status != WGPURequestDeviceStatus_Success)
  {
    std::cerr << "[WebGPU] Failed to get a Device" << std::endl;
    return;
  }
  data.device = device;
}

void UncapturedError(WGPUDevice const* device, WGPUErrorType type,
                     WGPUStringView message, void* userdata1, void* userdata2)
{
  std::cerr << "[WebGPU] " << WGPUToString(message) << std::endl;
}

void LostCallback(WGPUDevice const* device, WGPUDeviceLostReason reason,
                  WGPUStringView message, void* userdata1, void* userdata2)
{
  std::cerr << "[WebGPU] Device Lost: " << WGPUToString(message) << std::endl;
}
}  // namespace engine::wgpu
