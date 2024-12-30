#include "shader_module.h"

#include <stdio.h>

FL_ShaderModule FL_ShaderModuleCreate(
  const FL_Device* device, const WGPUShaderModuleDescriptor* descriptor)
{
  WGPUShaderModule handle =
    wgpuDeviceCreateShaderModule(device->handle, descriptor);
  if (!handle)
  {
    perror("[WebGPU] Could not create Shader Module\n");
    return (FL_ShaderModule){0};
  }

  return (FL_ShaderModule){.handle = handle};
}

void FL_ShaderModuleRelease(FL_ShaderModule* shaderModule)
{
  wgpuShaderModuleRelease(shaderModule->handle);
}
