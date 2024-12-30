#ifndef FL_QUEUE_HEADER_DEFINE
#define FL_QUEUE_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "engine/wgpu/buffer.h"
#include "engine/wgpu/device.h"

typedef struct
{
  WGPUQueue handle;
} FL_Queue;

FL_Queue FL_QueueCreate(WGPUQueue handle);
void FL_QueueRelease(FL_Queue* queue);

void FL_QueueSubmit(const FL_Queue* queue, size_t numCommands,
                    const WGPUCommandBuffer* commands);

void FL_QueueWriteBuffer(const FL_Queue* queue, const FL_Buffer* buffer,
                         uint64_t size, const void* data, uint64_t offset);

FL_Queue FL_DeviceGetQueue(const FL_Device* device);

#endif
