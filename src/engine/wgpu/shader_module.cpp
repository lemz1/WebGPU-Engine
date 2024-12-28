#include "shader_module.h"

namespace engine::wgpu
{
ShaderModule::ShaderModule(const Device& device,
                           const WGPUShaderModuleDescriptor* descriptor)
{
  _handle = wgpuDeviceCreateShaderModule(device, descriptor);
}

ShaderModule::~ShaderModule()
{
  wgpuShaderModuleRelease(_handle);
}
}  // namespace engine::wgpu
