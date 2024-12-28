#pragma once

#include <webgpu/webgpu.h>

#include "engine/wgpu/device.h"

namespace engine::wgpu
{
class ShaderModule
{
 public:
  explicit ShaderModule(WGPUShaderModule handle) : _handle(handle)
  {
  }
  explicit ShaderModule(const Device& device,
                        const WGPUShaderModuleDescriptor* descriptor);
  ~ShaderModule();

  WGPUShaderModule GetHandle() const
  {
    return _handle;
  }

  operator WGPUShaderModule() const
  {
    return _handle;
  }

 private:
  WGPUShaderModule _handle;
};
}  // namespace engine::wgpu
