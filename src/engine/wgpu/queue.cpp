#include "queue.h"

namespace engine::wgpu
{
Queue::Queue(WGPUQueue handle) : _handle(handle)
{
}

Queue Queue::FromDevice(const Device& device)
{
  WGPUQueue queue = wgpuDeviceGetQueue(device);
  return Queue(queue);
}

Queue::~Queue()
{
  wgpuQueueRelease(_handle);
}
}  // namespace engine::wgpu
