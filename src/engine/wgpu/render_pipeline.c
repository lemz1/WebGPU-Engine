#include "render_pipeline.h"

#include <stdio.h>
#include <stdlib.h>

#include "engine/wgpu/shader_module.h"
#include "engine/wgpu/util.h"

FL_RenderPipeline FL_RenderPipelineCreate(
  const FL_Device* device,
  const FL_RenderPipelineDescriptor* descriptor
)
{
  WGPUShaderSourceWGSL wgslSource = {
    .code = FL_StrToWGPU(descriptor->shader),
    .chain =
      {
        .next = NULL,
        .sType = WGPUSType_ShaderSourceWGSL,
      },
  };

  WGPUShaderModuleDescriptor shaderDesc = {
    .nextInChain = &wgslSource.chain,
    .label = FL_StrToWGPU("Shader Module"),
  };

  FL_ShaderModule shaderModule = FL_ShaderModuleCreate(device, &shaderDesc);

  WGPUBlendState blendState = {
    .color =
      {
        .srcFactor = WGPUBlendFactor_SrcAlpha,
        .dstFactor = WGPUBlendFactor_OneMinusSrcAlpha,
        .operation = WGPUBlendOperation_Add,
      },
    .alpha =
      {
        .srcFactor = WGPUBlendFactor_Zero,
        .dstFactor = WGPUBlendFactor_One,
        .operation = WGPUBlendOperation_Add,
      },
  };

  WGPUColorTargetState colorTarget = {
    .nextInChain = NULL,
    .format = descriptor->format,
    .blend = descriptor->useBlending ? &blendState : NULL,
    .writeMask = WGPUColorWriteMask_All,
  };

  WGPUFragmentState fragmentState = {
    .module = shaderModule.handle,
    .entryPoint = FL_StrToWGPU("fs_main"),
    .constantCount = 0,
    .constants = NULL,
    .targetCount = 1,
    .targets = &colorTarget,
  };

  WGPUVertexBufferLayout vertexBufferLayout = {
    .attributeCount = descriptor->numVertexAttributes,
    .attributes = descriptor->vertexAttributes,
    .stepMode = WGPUVertexStepMode_Vertex,
    .arrayStride = descriptor->vertexStride,
  };

  size_t numBindings = 0;
  if (descriptor->vertexUniform)
  {
    numBindings++;
  }
  if (descriptor->fragmentUniform)
  {
    numBindings++;
  }

  WGPUBindGroupLayoutEntry* bindGroupLayoutEntries =
    malloc(numBindings * sizeof(WGPUBindGroupLayoutEntry));

  WGPUBindGroupEntry* bindGroupEntries =
    malloc(numBindings * sizeof(WGPUBindGroupEntry));

  size_t entryIndex = 0;
  if (descriptor->vertexUniform)
  {
    bindGroupLayoutEntries[entryIndex] = (WGPUBindGroupLayoutEntry){
      .binding = descriptor->vertexUniform->binding,
      .visibility = WGPUShaderStage_Vertex,
      .buffer =
        {
          .type = WGPUBufferBindingType_Uniform,
          .minBindingSize = descriptor->vertexUniform->stride,
          .hasDynamicOffset = descriptor->vertexUniform->hasDynamicOffset,
        },
    };

    bindGroupEntries[entryIndex] = (WGPUBindGroupEntry){
      .binding = descriptor->vertexUniform->binding,
      .buffer = descriptor->vertexUniform->buffer->handle,
      .offset = 0,
      .size = descriptor->vertexUniform->stride,
    };

    entryIndex++;
  }

  if (descriptor->fragmentUniform)
  {
    bindGroupLayoutEntries[entryIndex] = (WGPUBindGroupLayoutEntry){
      .binding = descriptor->fragmentUniform->binding,
      .visibility = WGPUShaderStage_Fragment,
      .buffer =
        {
          .type = WGPUBufferBindingType_Uniform,
          .minBindingSize = descriptor->fragmentUniform->stride,
          .hasDynamicOffset = descriptor->fragmentUniform->hasDynamicOffset,
        },
    };

    bindGroupEntries[entryIndex] = (WGPUBindGroupEntry){
      .binding = descriptor->fragmentUniform->binding,
      .buffer = descriptor->fragmentUniform->buffer->handle,
      .offset = 0,
      .size = descriptor->fragmentUniform->stride,
    };

    entryIndex++;
  }

  WGPUBindGroupLayoutDescriptor bindGroupLayoutDescriptor = {
    .entries = bindGroupLayoutEntries,
    .entryCount = numBindings,
    .label = FL_StrToWGPU("Bind Group Layout"),
  };
  WGPUBindGroupLayout bindGroupLayout =
    wgpuDeviceCreateBindGroupLayout(device->handle, &bindGroupLayoutDescriptor);

  WGPUBindGroupDescriptor bindGroupDescriptor = {
    .layout = bindGroupLayout,
    .entryCount = numBindings,
    .entries = bindGroupEntries,
    .label = FL_StrToWGPU("Bind Group"),
  };

  WGPUBindGroup bindGroup =
    wgpuDeviceCreateBindGroup(device->handle, &bindGroupDescriptor);

  WGPUPipelineLayoutDescriptor pipelineLayoutDescriptor = {
    .bindGroupLayoutCount = 1,
    .bindGroupLayouts = &bindGroupLayout,
    .label = FL_StrToWGPU("Pipeline Layout"),
  };

  WGPUPipelineLayout pipelineLayout =
    wgpuDeviceCreatePipelineLayout(device->handle, &pipelineLayoutDescriptor);
  wgpuBindGroupLayoutRelease(bindGroupLayout);

  WGPURenderPipelineDescriptor pipelineDescriptor = {
    .label = FL_StrToWGPU("Pipeline"),
    .vertex =
      {
        .bufferCount = 1,
        .buffers = &vertexBufferLayout,
        .constantCount = 0,
        .constants = 0,
        .entryPoint = FL_StrToWGPU("vs_main"),
        .module = shaderModule.handle,
      },
    .primitive =
      {
        .topology = descriptor->topology,
        .stripIndexFormat = WGPUIndexFormat_Undefined,
        .frontFace = WGPUFrontFace_CCW,
        .cullMode = descriptor->cullMode,
      },
    .fragment = &fragmentState,
    .depthStencil = NULL,
    .multisample =
      {
        .count = 1,
        .mask = ~0u,
      },
    .layout = pipelineLayout,
  };

  WGPURenderPipeline pipeline =
    wgpuDeviceCreateRenderPipeline(device->handle, &pipelineDescriptor);
  wgpuPipelineLayoutRelease(pipelineLayout);
  FL_ShaderModuleRelease(&shaderModule);

  return (FL_RenderPipeline){
    .handle = pipeline,
    .bindGroup = bindGroup,
  };
}

void FL_RenderPipelineRelease(FL_RenderPipeline* pipeline)
{
  wgpuRenderPipelineRelease(pipeline->handle);
  wgpuBindGroupRelease(pipeline->bindGroup);
}
