#include "command_buffer.h"

FL_CommandBuffer FL_CommandBufferCreate(WGPUCommandBuffer handle)
{
  return (FL_CommandBuffer){.handle = handle};
}

void FL_CommandBufferRelease(FL_CommandBuffer* command)
{
  wgpuCommandBufferRelease(command->handle);
}
