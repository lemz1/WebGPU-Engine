#include "shader_module.h"

FL_ShaderModule FL_ShaderModuleCreate(
  const FL_Device* device, const WGPUShaderModuleDescriptor* descriptor)
{
  WGPUShaderModule handle =
    wgpuDeviceCreateShaderModule(device->handle, descriptor);

  return (FL_ShaderModule){.handle = handle};
}

void FL_ShaderModuleRelease(FL_ShaderModule* shaderModule)
{
  wgpuShaderModuleRelease(shaderModule->handle);
}
