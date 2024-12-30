#include "device.h"

#include <iostream>

#include "engine/wgpu/queue.h"
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

Device::Device(const Adapter& adapter)
{
  _requiredLimits = GetRequiredLimits(adapter);

  WGPUDeviceDescriptor descriptor{
    .nextInChain = nullptr,
    .label = StrToWGPU("Device"),
    .requiredLimits = &_requiredLimits,
    .defaultQueue =
      {
        .nextInChain = nullptr,
        .label = StrToWGPU("Default Queue"),
      },
    .deviceLostCallbackInfo2 =
      {
        .nextInChain = nullptr,
        .mode = WGPUCallbackMode_WaitAnyOnly,
        .callback = LostCallback,
        .userdata1 = nullptr,
        .userdata2 = nullptr,
      },
    .uncapturedErrorCallbackInfo2 =
      {
        .nextInChain = nullptr,
        .callback = UncapturedError,
        .userdata1 = nullptr,
        .userdata2 = nullptr,
      },
  };

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

Queue Device::GetQueue() const
{
  return Queue(wgpuDeviceGetQueue(_handle));
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

WGPURequiredLimits GetRequiredLimits(const Adapter& adapter)
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
        .maxBindGroups = 1,
        .maxBindGroupsPlusVertexBuffers = WGPU_LIMIT_U32_UNDEFINED,
        .maxBindingsPerBindGroup = WGPU_LIMIT_U32_UNDEFINED,
        .maxDynamicUniformBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED,
        .maxDynamicStorageBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED,
        .maxSampledTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxSamplersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxUniformBuffersPerShaderStage = 1,
        .maxUniformBufferBindingSize = 16 * sizeof(float),
        .maxStorageBufferBindingSize = WGPU_LIMIT_U64_UNDEFINED,
        .minUniformBufferOffsetAlignment =
          supportedLimits.limits.minUniformBufferOffsetAlignment,
        .minStorageBufferOffsetAlignment =
          supportedLimits.limits.minStorageBufferOffsetAlignment,
        .maxVertexBuffers = 2,
        .maxBufferSize = 1024 * 1024 * 16,
        .maxVertexAttributes = 3,
        .maxVertexBufferArrayStride = 128 * sizeof(float),
        .maxInterStageShaderComponents = 6,
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
