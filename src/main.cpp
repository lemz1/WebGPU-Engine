#include <iostream>

#include "engine/core/glfw_context.h"
#include "engine/core/window.h"
#include "engine/wgpu/adapter.h"
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

int main()
{
  auto instance = Instance();
  auto adapter = Adapter(instance);

  WGPUDeviceDescriptor deviceDescriptor{
      .label = StrToWGPU("Device"),
      .requiredFeatureCount = 0,
      .requiredFeatures = nullptr,
      .defaultQueue =
          {
              .nextInChain = nullptr,
              .label = StrToWGPU("Default Queue"),
          },
  };
  auto device = Device(adapter, deviceDescriptor);

  auto queue = Queue::FromDevice(device);

  auto glfwContext = engine::core::GLFWContext();
  auto window = engine::core::Window(glfwContext, 1280, 720, "WebGPU Engine");

  auto surface = Surface(instance, device, window);

  auto source = StrToWGPU(R"(
    @vertex
    fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {
        var p = vec2f(0.0, 0.0);
        if (in_vertex_index == 0u) {
            p = vec2f(-0.5, -0.5);
        } else if (in_vertex_index == 1u) {
            p = vec2f(0.5, -0.5);
        } else {
            p = vec2f(0.0, 0.5);
        }
        return vec4f(p, 0.0, 1.0);
    }

    @fragment
    fn fs_main() -> @location(0) vec4f {
        return vec4f(0.0, 0.4, 1.0, 1.0);
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
  colorTarget.format = WGPUTextureFormat_BGRA8Unorm;
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

  auto pipeline = RenderPipeline(device, &pipelineDesc);

  float time = 0.0f;

  while (!window.ShouldClose())
  {
    glfwContext.PollEvents();

    float newTime = (float)glfwContext.GetTime();
    float deltaTime = newTime - time;
    time = newTime;

    auto textureView = surface.GetNextTextureView();

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

    RenderPassEncoder renderPass = encoder.BeginRenderPass(&renderPassDesc);
    renderPass.SetPipeline(pipeline);
    renderPass.Draw(3);
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
