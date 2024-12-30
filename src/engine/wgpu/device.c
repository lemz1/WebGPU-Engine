#include "device.h"

#include <stdbool.h>
#include <stdio.h>

#include "engine/wgpu/queue.h"
#include "engine/wgpu/util.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef struct
{
  WGPUDevice device;
  bool finished;
} FL_DeviceUserData;

static void RequestDevice(WGPURequestDeviceStatus status, WGPUDevice device,
                          WGPUStringView message, void* userdata);

static void UncapturedError(WGPUDevice const* device, WGPUErrorType type,
                            struct WGPUStringView message, void* userdata1,
                            void* userdata2);

static void LostCallback(WGPUDevice const* device, WGPUDeviceLostReason reason,
                         struct WGPUStringView message, void* userdata1,
                         void* userdata2);

static WGPURequiredLimits GetRequiredLimits(const FL_Adapter* adapter);

FL_Device FL_DeviceCreate(const FL_Adapter* adapter)
{
  WGPURequiredLimits requiredLimits = GetRequiredLimits(adapter);

  WGPUDeviceDescriptor descriptor = {
    .nextInChain = NULL,
    .label = FL_StrToWGPU("Device"),
    .requiredLimits = &requiredLimits,
    .defaultQueue =
      {
        .nextInChain = NULL,
        .label = FL_StrToWGPU("Default Queue"),
      },
    .deviceLostCallbackInfo2 =
      {
        .nextInChain = NULL,
        .mode = WGPUCallbackMode_WaitAnyOnly,
        .callback = LostCallback,
        .userdata1 = NULL,
        .userdata2 = NULL,
      },
    .uncapturedErrorCallbackInfo2 =
      {
        .nextInChain = NULL,
        .callback = UncapturedError,
        .userdata1 = NULL,
        .userdata2 = NULL,
      },
  };

  FL_DeviceUserData data = {
    .device = NULL,
    .finished = false,
  };
  wgpuAdapterRequestDevice(adapter->handle, &descriptor, RequestDevice, &data);

#ifdef __EMSCRIPTEN__
  while (!data.finished)
  {
    emscripten_sleep(100);
  }
#endif

  if (!data.device)
  {
    perror("[WebGPU] Could not request Device\n");
    return (FL_Device){0};
  }
  WGPUDevice handle = data.device;

  return (FL_Device){
    .requiredLimits = requiredLimits,
    .handle = handle,
  };
}

void FL_DeviceRelease(FL_Device* device)
{
  wgpuDeviceRelease(device->handle);
}

void FL_DeviceTick(const FL_Device* device)
{
  wgpuDeviceTick(device->handle);
}

FL_Queue FL_DeviceGetQueue(const FL_Device* device)
{
  return FL_QueueCreate(wgpuDeviceGetQueue(device->handle));
}

static void RequestDevice(WGPURequestDeviceStatus status, WGPUDevice device,
                          WGPUStringView message, void* userdata)
{
  FL_DeviceUserData* data = (FL_DeviceUserData*)userdata;
  data->finished = true;
  if (status != WGPURequestDeviceStatus_Success)
  {
    fprintf(stderr, "[WebGPU] Failed to get a Device: %.*s\n",
            (int)message.length, message.data);
    return;
  }
  data->device = device;
}

void UncapturedError(WGPUDevice const* device, WGPUErrorType type,
                     WGPUStringView message, void* userdata1, void* userdata2)
{
  fprintf(stderr, "[WEBGPU]: %.*s\n", (int)message.length, message.data);
}

void LostCallback(WGPUDevice const* device, WGPUDeviceLostReason reason,
                  WGPUStringView message, void* userdata1, void* userdata2)
{
  printf("[WebGPU] Device Lost %.*s\n", (int)message.length, message.data);
}

WGPURequiredLimits GetRequiredLimits(const FL_Adapter* adapter)
{
  WGPUSupportedLimits supportedLimits = {
    .nextInChain = NULL,
  };
  wgpuAdapterGetLimits(adapter->handle, &supportedLimits);

  WGPURequiredLimits requiredLimits = {
    .nextInChain = NULL,
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
