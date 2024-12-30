#include <iostream>

#include "engine/core/glfw_context.h"
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

using namespace engine::wgpu;
using namespace engine::core;

int main()
{
  auto instance = Instance();
  auto adapter = Adapter(instance);
  auto device = Device(adapter);
  Queue queue = device.GetQueue();

  auto glfwContext = GLFWContext();
  auto window = Window(glfwContext, 1280, 720, "WebGPU Engine");

  auto surface = Surface(instance, adapter, device, window);

  std::vector<float> vertices = {
    -0.5, -0.5, +0.0, +0.0,  //
    +0.5, -0.5, +1.0, +0.0,  //
    -0.5, +0.5, +0.0, +1.0,  //
    +0.5, +0.5, +1.0, +1.0,  //
  };

  auto vertexBuffer = Buffer(device, vertices.size() * sizeof(float),
                             WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex);
  queue.WriteBuffer(vertexBuffer, vertexBuffer.GetSize(), vertices.data());

  std::vector<uint32_t> indices = {
    0, 1, 2,  //
    1, 3, 2,  //
  };

  auto indexBuffer = Buffer(device, indices.size() * sizeof(uint32_t),
                            WGPUBufferUsage_CopyDst | WGPUBufferUsage_Index);
  queue.WriteBuffer(indexBuffer, indexBuffer.GetSize(), indices.data());

  float uniformData = 0.0f;

  auto uniformBuffer = Buffer(
    device, sizeof(float), WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform);
  queue.WriteBuffer(uniformBuffer, uniformBuffer.GetSize(), &uniformData);

  auto source = StrToWGPU(R"(
    struct VertexInput {
      @location(0) position: vec2f,
      @location(1) texCoord: vec2f,
    };

    struct VertexOutput {
      @builtin(position) position: vec4f,
      @location(0) texCoord: vec2f,
    };

    @group(0) @binding(0) var<uniform> uTime: f32;

    @vertex
    fn vs_main(in: VertexInput) -> VertexOutput {
      var out: VertexOutput;
      out.position = vec4f(in.position.x, in.position.y + sin(uTime) * 0.5, 0.0, 1.0);
      out.texCoord = in.texCoord;
      return out;
    }

    @fragment
    fn fs_main(in: VertexOutput) -> @location(0) vec4f {
        return vec4f(in.texCoord, 1.0, 1.0);
    }
  )");

  WGPUShaderSourceWGSL wgslSource{};
  wgslSource.code = source;
  wgslSource.chain = {
    .next = nullptr,
    .sType = WGPUSType_ShaderSourceWGSL,
  };

  WGPUShaderModuleDescriptor shaderDesc{};
  shaderDesc.nextInChain = &wgslSource.chain;
  shaderDesc.label = StrToWGPU("Shader Module");

  auto shaderModule = ShaderModule(device, &shaderDesc);

  WGPURenderPipelineDescriptor pipelineDesc{};
  pipelineDesc.nextInChain = nullptr;
  pipelineDesc.vertex.bufferCount = 0;
  pipelineDesc.vertex.buffers = nullptr;
  pipelineDesc.vertex.module = shaderModule;
  pipelineDesc.vertex.entryPoint = StrToWGPU("vs_main");
  pipelineDesc.vertex.constantCount = 0;
  pipelineDesc.vertex.constants = nullptr;
  pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
  pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
  pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
  pipelineDesc.primitive.cullMode = WGPUCullMode_None;

  WGPUFragmentState fragmentState{};
  fragmentState.module = shaderModule;
  fragmentState.entryPoint = StrToWGPU("fs_main");
  fragmentState.constantCount = 0;
  fragmentState.constants = nullptr;

  WGPUBlendState blendState{};
  blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
  blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
  blendState.color.operation = WGPUBlendOperation_Add;
  blendState.alpha.srcFactor = WGPUBlendFactor_Zero;
  blendState.alpha.dstFactor = WGPUBlendFactor_One;
  blendState.alpha.operation = WGPUBlendOperation_Add;

  WGPUColorTargetState colorTarget{};
  colorTarget.format = surface.GetPreferredFormat();
  colorTarget.blend = &blendState;
  colorTarget.writeMask = WGPUColorWriteMask_All;

  fragmentState.targetCount = 1;
  fragmentState.targets = &colorTarget;

  pipelineDesc.fragment = &fragmentState;

  pipelineDesc.depthStencil = nullptr;
  pipelineDesc.multisample.count = 1;
  pipelineDesc.multisample.mask = ~0u;
  pipelineDesc.multisample.alphaToCoverageEnabled = false;
  pipelineDesc.layout = nullptr;
  pipelineDesc.label = StrToWGPU("Pipeline");

  // Vertex fetch
  WGPUVertexBufferLayout vertexBufferLayout{};

  std::vector<WGPUVertexAttribute> attributes = {
    WGPUVertexAttribute{
      .format = WGPUVertexFormat_Float32x2,
      .offset = 0,
      .shaderLocation = 0,
    },
    WGPUVertexAttribute{
      .format = WGPUVertexFormat_Float32x2,
      .offset = 2 * sizeof(float),
      .shaderLocation = 1,
    },
  };

  vertexBufferLayout.attributeCount = attributes.size();
  vertexBufferLayout.attributes = attributes.data();

  vertexBufferLayout.arrayStride = 4 * sizeof(float);
  vertexBufferLayout.stepMode = WGPUVertexStepMode_Vertex;

  pipelineDesc.vertex.bufferCount = 1;
  pipelineDesc.vertex.buffers = &vertexBufferLayout;

  // Define binding layout
  WGPUBindGroupLayoutEntry bindingLayout{};
  bindingLayout.binding = 0;
  bindingLayout.visibility = WGPUShaderStage_Vertex;
  bindingLayout.buffer.type = WGPUBufferBindingType_Uniform;
  bindingLayout.buffer.minBindingSize = sizeof(float);

  WGPUBindGroupLayoutDescriptor bindGroupLayoutDesc{};
  bindGroupLayoutDesc.entryCount = 1;
  bindGroupLayoutDesc.entries = &bindingLayout;
  WGPUBindGroupLayout bindGroupLayout =
    wgpuDeviceCreateBindGroupLayout(device, &bindGroupLayoutDesc);

  WGPUBindGroupEntry binding{};
  binding.nextInChain = nullptr;
  binding.binding = 0;
  binding.buffer = uniformBuffer;
  binding.offset = 0;
  binding.size = sizeof(float);

  WGPUBindGroupDescriptor bindGroupDesc{};
  bindGroupDesc.nextInChain = nullptr;
  bindGroupDesc.layout = bindGroupLayout;
  bindGroupDesc.entryCount = 1;
  bindGroupDesc.entries = &binding;
  WGPUBindGroup bindGroup = wgpuDeviceCreateBindGroup(device, &bindGroupDesc);

  // Create the pipeline layout
  WGPUPipelineLayoutDescriptor layoutDesc{};
  layoutDesc.bindGroupLayoutCount = 1;
  layoutDesc.bindGroupLayouts = (WGPUBindGroupLayout*)&bindGroupLayout;
  WGPUPipelineLayout layout =
    wgpuDeviceCreatePipelineLayout(device, &layoutDesc);
  pipelineDesc.layout = layout;

  auto pipeline = RenderPipeline(device, &pipelineDesc);

  float time = 0.0f;

  while (!window.ShouldClose())
  {
    glfwContext.PollEvents();

    float newTime = (float)glfwContext.GetTime();
    float deltaTime = newTime - time;
    time = newTime;

    auto textureView = surface.GetNextTextureView();

    queue.WriteBuffer(uniformBuffer, uniformBuffer.GetSize(), &time);

    auto encoder = CommandEncoder(device);

    WGPURenderPassDescriptor renderPassDesc = {};
    renderPassDesc.nextInChain = nullptr;

    WGPURenderPassColorAttachment renderPassColorAttachment = {};
    renderPassColorAttachment.view = textureView;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
    renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
    renderPassColorAttachment.clearValue = WGPUColor{0.1, 0.1, 0.1, 1.0};
    renderPassColorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &renderPassColorAttachment;
    renderPassDesc.depthStencilAttachment = nullptr;
    renderPassDesc.timestampWrites = nullptr;
    renderPassDesc.label = StrToWGPU("Render Pass");

    RenderPassEncoder renderPass = encoder.BeginRenderPass(&renderPassDesc);
    renderPass.SetVertexBuffer(vertexBuffer, 0, vertexBuffer.GetSize());
    renderPass.SetIndexBuffer(indexBuffer, indexBuffer.GetSize());
    renderPass.SetPipeline(pipeline);
    wgpuRenderPassEncoderSetBindGroup(renderPass, 0, bindGroup, 0, nullptr);
    renderPass.DrawIndexed(indices.size());
    renderPass.End();

    CommandBuffer command = encoder.Finish();

    queue.Submit({command});

#ifndef __EMSCRIPTEN__
    surface.Present();
#endif

    device.Tick();
  }

  return 0;
}
