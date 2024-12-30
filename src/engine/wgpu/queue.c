#include "queue.h"

FL_Queue FL_QueueCreate(WGPUQueue handle)
{
  return (FL_Queue){.handle = handle};
}
void FL_QueueRelease(FL_Queue* queue)
{
  wgpuQueueRelease(queue->handle);
}

void FL_QueueSubmit(const FL_Queue* queue, size_t numCommands,
                    const WGPUCommandBuffer* commands)
{
  wgpuQueueSubmit(queue->handle, numCommands, commands);
}

void FL_QueueWriteBuffer(const FL_Queue* queue, const FL_Buffer* buffer,
                         uint64_t size, const void* data, uint64_t offset)
{
  wgpuQueueWriteBuffer(queue->handle, buffer->handle, offset, data, size);
}
