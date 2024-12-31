#ifndef FL_BIND_GROUP_HEADER_DEFINE
#define FL_BIND_GROUP_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "engine/wgpu/device.h"

typedef struct
{
  WGPUBindGroup handle;
  WGPUBindGroupLayout layout;
} FL_BindGroup;

FL_BindGroup FL_BindGroupCreate(const FL_Device* device, size_t numEntries,
                                const WGPUBindGroupLayoutEntry* layoutEntries,
                                const WGPUBindGroupEntry* entries);
void FL_BindGroupRelease(FL_BindGroup* group);

#endif
