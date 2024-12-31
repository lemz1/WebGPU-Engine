#include <stdio.h>

#include "engine/core/window.h"
#include "engine/wgpu/adapter.h"
#include "engine/wgpu/buffer.h"
#include "engine/wgpu/command_buffer.h"
#include "engine/wgpu/command_encoder.h"
#include "engine/wgpu/device.h"
#include "engine/wgpu/instance.h"
#include "engine/wgpu/queue.h"
#include "engine/wgpu/render_pass_encoder.h"
#include "engine/wgpu/render_pipeline.h"
#include "engine/wgpu/shader_module.h"
#include "engine/wgpu/surface.h"
#include "engine/wgpu/util.h"

int main()
{
  FL_Instance instance = FL_InstanceCreate();
  FL_Adapter adapter = FL_AdapterCreate(&instance);
  FL_Device device = FL_DeviceCreate(&adapter);
  FL_Queue queue = FL_DeviceGetQueue(&device);

  if (!glfwInit())
  {
    return 1;
  }

  FL_Window window = FL_WindowCreate(1280, 720, "WebGPU Engine");

  FL_Surface surface = FL_SurfaceCreate(&instance, &adapter, &device, &window);

  float vertices[16] = {
    -0.5,
    -0.5,
    +0.0,
    +0.0,
    //
    +0.5,
    -0.5,
    +1.0,
    +0.0,
    //
    -0.5,
    +0.5,
    +0.0,
    +1.0,
    //
    +0.5,
    +0.5,
    +1.0,
    +1.0,
    //
  };

  FL_Buffer vertexBuffer = FL_BufferCreate(
    &device,
    sizeof(vertices),
    WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex
  );
  FL_QueueWriteBuffer(&queue, &vertexBuffer, vertexBuffer.size, vertices, 0);

  uint32_t indices[6] = {
    0,
    1,
    2,
    //
    1,
    3,
    2,
    //
  };

  FL_Buffer indexBuffer = FL_BufferCreate(
    &device,
    sizeof(indices),
    WGPUBufferUsage_CopyDst | WGPUBufferUsage_Index
  );
  FL_QueueWriteBuffer(&queue, &indexBuffer, indexBuffer.size, indices, 0);

  float uniformData = 0.0f;

  FL_Buffer uniformBuffer = FL_BufferCreate(
    &device,
    sizeof(float),
    WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform
  );
  FL_QueueWriteBuffer(
    &queue,
    &uniformBuffer,
    uniformBuffer.size,
    &uniformData,
    0
  );

  const char* source =
    "struct VertexInput {\n"
    "  @location(0) position: vec2f,\n"
    "@location(1) texCoord: vec2f,\n"
    "};\n"

    "struct VertexOutput {"
    "@builtin(position) position: vec4f,"
    "@location(0) texCoord: vec2f,"
    "};\n"

    "@group(0) @binding(0) var<uniform> uTime: f32;\n"

    "@vertex\n"
    "fn vs_main(in: VertexInput) -> VertexOutput {"
    "var out: VertexOutput;"
    "out.position = vec4f(in.position.x, in.position.y + "
    "sin(uTime) * 0.5, "
    "0.0, 1.0);"
    "out.texCoord = in.texCoord;"
    "return out;"
    "}\n"

    "@fragment\n"
    "fn fs_main(in: VertexOutput) -> @location(0) vec4f {"
    "return vec4f(in.texCoord, 1.0, 1.0);"
    "}";

  WGPUVertexAttribute vertexAttributes[2] = {
    (WGPUVertexAttribute){
      .format = WGPUVertexFormat_Float32x2,
      .offset = 0,
      .shaderLocation = 0,
    },
    (WGPUVertexAttribute){
      .format = WGPUVertexFormat_Float32x2,
      .offset = 2 * sizeof(float),
      .shaderLocation = 1,
    },
  };

  FL_RenderPipelineUniform vertexUniform = {
    .buffer = &uniformBuffer,
    .binding = 0,
    .stride = sizeof(float),
    .hasDynamicOffset = false,
  };

  FL_RenderPipelineDescriptor pipelineDescriptor = {
    .shader = source,
    .vertexStride = 4 * sizeof(float),
    .numVertexAttributes = 2,
    .vertexAttributes = vertexAttributes,
    .vertexUniform = &vertexUniform,
    .fragmentUniform = NULL,
    .topology = WGPUPrimitiveTopology_TriangleList,
    .cullMode = WGPUCullMode_None,
    .format = surface.format,
    .useBlending = true,
  };

  FL_RenderPipeline pipeline =
    FL_RenderPipelineCreate(&device, &pipelineDescriptor);

  float time = 0.0f;

  while (!FL_WindowShouldClose(&window))
  {
    glfwPollEvents();

    float newTime = (float)glfwGetTime();
    float deltaTime = newTime - time;
    time = newTime;

    FL_TextureView textureView = FL_SurfaceGetNextTextureView(&surface);

    FL_QueueWriteBuffer(&queue, &uniformBuffer, uniformBuffer.size, &time, 0);

    FL_CommandEncoder encoder = FL_CommandEncoderCreate(&device);

    WGPURenderPassDescriptor renderPassDesc = {0};
    renderPassDesc.nextInChain = NULL;

    WGPURenderPassColorAttachment renderPassColorAttachment = {0};
    renderPassColorAttachment.view = textureView.handle;
    renderPassColorAttachment.resolveTarget = NULL;
    renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
    renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
    renderPassColorAttachment.clearValue = (WGPUColor){0.1, 0.1, 0.1, 1.0};
    renderPassColorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &renderPassColorAttachment;
    renderPassDesc.depthStencilAttachment = NULL;
    renderPassDesc.timestampWrites = NULL;

    renderPassDesc.label = FL_StrToWGPU("Render Pass");

    FL_RenderPassEncoder renderPass =
      FL_CommandEncoderBeginRenderPass(&encoder, &renderPassDesc);

    FL_RenderPassEncoderSetVertexBuffer(
      &renderPass,
      &vertexBuffer,
      0,
      vertexBuffer.size,
      0
    );
    FL_RenderPassEncoderSetIndexBuffer(
      &renderPass,
      &indexBuffer,
      indexBuffer.size,
      WGPUIndexFormat_Uint32,
      0
    );
    FL_RenderPassEncoderSetPipeline(&renderPass, &pipeline);

    FL_RenderPassEncoderSetBindGroup(
      &renderPass,
      0,
      pipeline.bindGroup,
      0,
      NULL
    );
    FL_RenderPassEncoderDrawIndexed(&renderPass, 6, 1, 0, 0);
    FL_RenderPassEncoderEnd(&renderPass);

    FL_CommandBuffer command = FL_CommandEncoderFinish(&encoder);
    FL_RenderPassEncoderRelease(&renderPass);
    FL_CommandEncoderRelease(&encoder);

    FL_QueueSubmit(&queue, 1, &command.handle);
    FL_CommandBufferRelease(&command);

#ifndef __EMSCRIPTEN__
    FL_SurfacePresent(&surface);
#endif
    FL_TextureViewRelease(&textureView);

    FL_DeviceTick(&device);
  }

  FL_RenderPipelineRelease(&pipeline);
  FL_BufferRelease(&uniformBuffer);
  FL_BufferRelease(&indexBuffer);
  FL_BufferRelease(&vertexBuffer);
  FL_SurfaceRelease(&surface);
  FL_WindowDestroy(&window);
  glfwTerminate();
  FL_QueueRelease(&queue);
  FL_DeviceRelease(&device);
  FL_AdapterRelease(&adapter);
  FL_InstanceRelease(&instance);

  return 0;
}
