#include "buffer.h"

#include "engine/wgpu/util.h"

namespace engine::wgpu
{
Buffer::Buffer(const Device& device, uint64_t size, WGPUBufferUsage usage)
  : _size(size)
{
  WGPUBufferDescriptor bufferDesc = {
    .nextInChain = nullptr,
    .label = StrToWGPU("Buffer"),
    .usage = usage,
    .size = size,
    .mappedAtCreation = false,
  };
  _handle = wgpuDeviceCreateBuffer(device, &bufferDesc);
}

Buffer::~Buffer()
{
  wgpuBufferRelease(_handle);
}
}  // namespace engine::wgpu
