#ifndef FL_SURFACE_HEADER_DEFINE
#define FL_SURFACE_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "faceless/core/window.h"
#include "faceless/wgpu/adapter.h"
#include "faceless/wgpu/device.h"
#include "faceless/wgpu/instance.h"
#include "faceless/wgpu/texture_view.h"

typedef struct
{
  WGPUSurface handle;
  WGPUTextureFormat format;
} FL_Surface;

FL_Surface FL_SurfaceCreate(
  const FL_Instance* instance,
  const FL_Adapter* adapter,
  const FL_Device* device,
  const FL_Window* window
);
void FL_SurfaceRelease(FL_Surface* surface);

FL_TextureView FL_SurfaceGetNextTextureView(const FL_Surface* surface);

void FL_SurfacePresent(const FL_Surface* surface);

#endif
