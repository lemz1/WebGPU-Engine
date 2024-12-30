#ifndef FL_BUFFER_HEADER_DEFINE
#define FL_BUFFER_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "engine/wgpu/device.h"

typedef struct
{
  uint64_t size;
  WGPUBuffer handle;
} FL_Buffer;

FL_Buffer FL_BufferCreate(const FL_Device* device, uint64_t size,
                          WGPUBufferUsage usage);
void FL_BufferRelease(FL_Buffer* buffer);

#endif
