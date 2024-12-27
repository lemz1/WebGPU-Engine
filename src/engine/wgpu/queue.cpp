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

void Queue::Submit(const std::vector<const CommandBuffer*>& commands) const
{
  std::vector<WGPUCommandBuffer> handles;
  handles.reserve(commands.size());
  for (auto cmd : commands)
  {
    handles.emplace_back(*cmd);
  }
  wgpuQueueSubmit(_handle, handles.size(), handles.data());
}
}  // namespace engine::wgpu
