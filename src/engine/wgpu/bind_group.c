#include "bind_group.h"

#include "engine/wgpu/util.h"

FL_BindGroup FL_BindGroupCreate(const FL_Device* device, size_t numEntries,
                                const WGPUBindGroupLayoutEntry* layoutEntries,
                                const WGPUBindGroupEntry* entries)
{
  WGPUBindGroupLayoutDescriptor layoutDescriptor = {
    .entries = layoutEntries,
    .entryCount = numEntries,
    .nextInChain = NULL,
    .label = FL_StrToWGPU("Bind Group Layout"),
  };
  WGPUBindGroupLayout layout =
    wgpuDeviceCreateBindGroupLayout(device->handle, &layoutDescriptor);

  WGPUBindGroupDescriptor descriptor = {0};
  descriptor.nextInChain = NULL;
  descriptor.layout = layout;
  descriptor.entryCount = numEntries;
  descriptor.entries = entries;
  WGPUBindGroup bindGroup =
    wgpuDeviceCreateBindGroup(device->handle, &descriptor);

  return (FL_BindGroup){
    .handle = bindGroup,
    .layout = layout,
  };
}

void FL_BindGroupRelease(FL_BindGroup* group)
{
  wgpuBindGroupRelease(group->handle);
  wgpuBindGroupLayoutRelease(group->layout);
}
