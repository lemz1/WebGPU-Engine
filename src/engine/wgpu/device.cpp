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

static WGPURequiredLimits GetRequiredLimits(const Adapter& adapter);

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

  if (descriptor.requiredLimits == nullptr)
  {
    _requiredLimits = GetRequiredLimits(adapter);
    descriptor.requiredLimits = &_requiredLimits;
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

void Device::Tick() const
{
  wgpuDeviceTick(_handle);
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

static WGPURequiredLimits GetRequiredLimits(const Adapter& adapter)
{
  WGPUSupportedLimits supportedLimits{
    .nextInChain = nullptr,
  };
  wgpuAdapterGetLimits(adapter, &supportedLimits);

  WGPURequiredLimits requiredLimits{
    .nextInChain = nullptr,
    .limits =
      {
        .maxTextureDimension1D = WGPU_LIMIT_U32_UNDEFINED,
        .maxTextureDimension2D = WGPU_LIMIT_U32_UNDEFINED,
        .maxTextureDimension3D = WGPU_LIMIT_U32_UNDEFINED,
        .maxTextureArrayLayers = WGPU_LIMIT_U32_UNDEFINED,
        .maxBindGroups = WGPU_LIMIT_U32_UNDEFINED,
        .maxBindGroupsPlusVertexBuffers = WGPU_LIMIT_U32_UNDEFINED,
        .maxBindingsPerBindGroup = WGPU_LIMIT_U32_UNDEFINED,
        .maxDynamicUniformBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED,
        .maxDynamicStorageBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED,
        .maxSampledTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxSamplersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxUniformBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxUniformBufferBindingSize = WGPU_LIMIT_U64_UNDEFINED,
        .maxStorageBufferBindingSize = WGPU_LIMIT_U64_UNDEFINED,
        .minUniformBufferOffsetAlignment =
          supportedLimits.limits.minUniformBufferOffsetAlignment,
        .minStorageBufferOffsetAlignment =
          supportedLimits.limits.minStorageBufferOffsetAlignment,
        .maxVertexBuffers = 1,
        .maxBufferSize = 12 * 2 * sizeof(float),
        .maxVertexAttributes = 2,
        .maxVertexBufferArrayStride = 4 * sizeof(float),
        .maxInterStageShaderComponents = 3,
        .maxInterStageShaderVariables = WGPU_LIMIT_U32_UNDEFINED,
        .maxColorAttachments = WGPU_LIMIT_U32_UNDEFINED,
        .maxColorAttachmentBytesPerSample = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupStorageSize = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeInvocationsPerWorkgroup = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupSizeX = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupSizeY = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupSizeZ = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupsPerDimension = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageBuffersInVertexStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageTexturesInVertexStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageBuffersInFragmentStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageTexturesInFragmentStage = WGPU_LIMIT_U32_UNDEFINED,
      },
  };

  return requiredLimits;
}
}  // namespace engine::wgpu
