#include "surface.h"

#include <glfw3webgpu.h>

#include <iostream>

#include "engine/wgpu/util.h"

namespace engine::wgpu
{
Surface::Surface(const Instance& instance, const Adapter& adapter,
                 const Device& device, const engine::core::Window& window)
{
  _handle = glfwGetWGPUSurface(instance, window);

  WGPUSurfaceCapabilities capabilities{};
  if (wgpuSurfaceGetCapabilities(_handle, adapter, &capabilities) !=
      WGPUStatus_Success)
  {
    std::cerr << "[WebGPU] Could not get Surface Capabilities" << std::endl;
    return;
  }

  _formats.assign(capabilities.formats,
                  capabilities.formats + capabilities.formatCount);

  WGPUSurfaceConfiguration config{
    .nextInChain = nullptr,
    .device = device,
    .format = GetPreferredFormat(),
    .usage = WGPUTextureUsage_RenderAttachment,
    .viewFormatCount = 0,
    .viewFormats = nullptr,
    .alphaMode = WGPUCompositeAlphaMode_Auto,
    .width = 1280,
    .height = 720,
    .presentMode = WGPUPresentMode_Fifo,
  };
  wgpuSurfaceConfigure(_handle, &config);
}

Surface::~Surface()
{
  wgpuSurfaceUnconfigure(_handle);
  wgpuSurfaceRelease(_handle);
}

WGPUTextureFormat Surface::GetPreferredFormat() const
{
  return _formats[0];
}

TextureView Surface::GetNextTextureView() const
{
  WGPUSurfaceTexture surfaceTexture;
  wgpuSurfaceGetCurrentTexture(_handle, &surfaceTexture);

  if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success)
  {
    std::terminate();
  }

  WGPUTextureViewDescriptor viewDescriptor{
    .nextInChain = nullptr,
    .label = StrToWGPU("Surface texture view"),
    .format = wgpuTextureGetFormat(surfaceTexture.texture),
    .dimension = WGPUTextureViewDimension_2D,
    .baseMipLevel = 0,
    .mipLevelCount = 1,
    .baseArrayLayer = 0,
    .arrayLayerCount = 1,
    .aspect = WGPUTextureAspect_All,
  };

  WGPUTextureView textureView =
    wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);

  wgpuTextureRelease(surfaceTexture.texture);

  return TextureView(textureView);
}

void Surface::Present() const
{
  wgpuSurfacePresent(_handle);
}
}  // namespace engine::wgpu
