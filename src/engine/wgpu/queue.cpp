#include "queue.h"

#include <iostream>

namespace engine::wgpu
{
static void QueueWorkDone(WGPUQueueWorkDoneStatus status, void*, void*);

Queue Queue::FromDevice(const Device& device)
{
  WGPUQueue queue = wgpuDeviceGetQueue(device);
  return Queue(queue);
}

Queue::~Queue()
{
  wgpuQueueRelease(_handle);
}

void Queue::Submit(const std::vector<WGPUCommandBuffer>& commands) const
{
  wgpuQueueSubmit(_handle, commands.size(), commands.data());
}

WGPUFuture Queue::SubmitFuture(
  const std::vector<WGPUCommandBuffer>& commands) const
{
  wgpuQueueSubmit(_handle, commands.size(), commands.data());

  WGPUQueueWorkDoneCallbackInfo2 info{
    .nextInChain = nullptr,
    .mode = WGPUCallbackMode_WaitAnyOnly,
    .callback = QueueWorkDone,
    .userdata1 = nullptr,
    .userdata2 = nullptr,
  };
  return wgpuQueueOnSubmittedWorkDone2(_handle, info);
}

void Queue::WriteBuffer(const Buffer& buffer, uint64_t size, const void* data,
                        uint64_t offset) const
{
  wgpuQueueWriteBuffer(_handle, buffer, offset, data, size);
}

static void QueueWorkDone(WGPUQueueWorkDoneStatus status, void*, void*)
{
  std::cout << "[WebGPU] Finished queued work with status: " << status
            << std::endl;
}
}  // namespace engine::wgpu
