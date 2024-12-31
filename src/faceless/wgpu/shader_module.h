#ifndef FL_SHADER_MODULE_HEADER_DEFINE
#define FL_SHADER_MODULE_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "faceless/wgpu/device.h"

typedef struct
{
  WGPUShaderModule handle;
} FL_ShaderModule;

FL_ShaderModule FL_ShaderModuleCreate(
  const FL_Device* device,
  const WGPUShaderModuleDescriptor* descriptor
);
void FL_ShaderModuleRelease(FL_ShaderModule* shaderModule);

#endif
