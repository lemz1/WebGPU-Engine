#include "surface.h"

#include <glfw3webgpu.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine/wgpu/util.h"

FL_Surface FL_SurfaceCreate(const FL_Instance* instance,
                            const FL_Adapter* adapter, const FL_Device* device,
                            const FL_Window* window)
{
  WGPUSurface handle = glfwGetWGPUSurface(instance->handle, window->handle);
  if (!handle)
  {
    perror("[WebGPU] Could not create Surface\n");
    return (FL_Surface){0};
  }

  WGPUSurfaceCapabilities capabilities;
  if (wgpuSurfaceGetCapabilities(handle, adapter->handle, &capabilities) !=
      WGPUStatus_Success)
  {
    perror("[WebGPU] Could not get Surface Capabilities\n");
    return (FL_Surface){0};
  }

  WGPUSurfaceConfiguration config = {
    .nextInChain = NULL,
    .device = device->handle,
    .format = capabilities.formats[0],
    .usage = WGPUTextureUsage_RenderAttachment,
    .viewFormatCount = 0,
    .viewFormats = NULL,
    .alphaMode = WGPUCompositeAlphaMode_Auto,
    .width = 1280,
    .height = 720,
    .presentMode = WGPUPresentMode_Fifo,
  };
  wgpuSurfaceConfigure(handle, &config);

  return (FL_Surface){
    .handle = handle,
    .format = capabilities.formats[0],
  };
}

void FL_SurfaceRelease(FL_Surface* surface)
{
  wgpuSurfaceRelease(surface->handle);
}

FL_TextureView FL_SurfaceGetNextTextureView(const FL_Surface* surface)
{
  WGPUSurfaceTexture surfaceTexture;
  wgpuSurfaceGetCurrentTexture(surface->handle, &surfaceTexture);

  if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success)
  {
    return (FL_TextureView){0};
  }

  WGPUTextureViewDescriptor viewDescriptor = {
    .nextInChain = NULL,
    .label = FL_StrToWGPU("Surface Texture View"),
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

  return FL_TextureViewCreate(textureView);
}

void FL_SurfacePresent(const FL_Surface* surface)
{
  wgpuSurfacePresent(surface->handle);
}
