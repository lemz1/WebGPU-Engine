#ifndef FL_DEVICE_HEADER_DEFINE
#define FL_DEVICE_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "engine/wgpu/adapter.h"

typedef struct
{
  WGPURequiredLimits requiredLimits;
  WGPUDevice handle;
} FL_Device;

FL_Device FL_DeviceCreate(const FL_Adapter* adapter);
void FL_DeviceRelease(FL_Device* device);

void FL_DeviceTick(const FL_Device* device);

#endif
