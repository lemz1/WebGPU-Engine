#ifndef FL_COMMAND_ENCODER_HEADER_DEFINE
#define FL_COMMAND_ENCODER_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "engine/wgpu/command_buffer.h"
#include "engine/wgpu/device.h"
#include "engine/wgpu/render_pass_encoder.h"

typedef struct
{
  WGPUCommandEncoder handle;
} FL_CommandEncoder;

FL_CommandEncoder FL_CommandEncoderCreate(const FL_Device* device);
void FL_CommandEncoderRelease(FL_CommandEncoder* encoder);

FL_CommandBuffer FL_CommandEncoderFinish(const FL_CommandEncoder* encoder);

FL_RenderPassEncoder FL_CommandEncoderBeginRenderPass(
  const FL_CommandEncoder* encoder, const WGPURenderPassDescriptor* descriptor);

#endif
