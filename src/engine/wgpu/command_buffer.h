#ifndef FL_COMMAND_BUFFER_HEADER_DEFINE
#define FL_COMMAND_BUFFER_HEADER_DEFINE

#include <webgpu/webgpu.h>

typedef struct
{
  WGPUCommandBuffer handle;
} FL_CommandBuffer;

FL_CommandBuffer FL_CommandBufferCreate(WGPUCommandBuffer handle);
void FL_CommandBufferRelease(FL_CommandBuffer* command);

#endif
