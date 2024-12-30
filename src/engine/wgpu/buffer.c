#include "buffer.h"

#include <stdbool.h>

#include "engine/wgpu/util.h"

FL_Buffer FL_BufferCreate(const FL_Device* device, uint64_t size,
                          WGPUBufferUsage usage)
{
  WGPUBufferDescriptor bufferDesc = {
    .nextInChain = NULL,
    .label = FL_StrToWGPU("Buffer"),
    .usage = usage,
    .size = size,
    .mappedAtCreation = false,
  };
  WGPUBuffer handle = wgpuDeviceCreateBuffer(device->handle, &bufferDesc);

  return (FL_Buffer){
    .size = size,
    .handle = handle,
  };
}

void FL_BufferRelease(FL_Buffer* buffer)
{
  wgpuBufferRelease(buffer->handle);
}
