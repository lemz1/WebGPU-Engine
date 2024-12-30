#ifndef FL_TEXTURE_VIEW_HEADER_DEFINE
#define FL_TEXTURE_VIEW_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "engine/wgpu/util.h"

typedef struct
{
  WGPUTextureView handle;
} FL_TextureView;

FL_TextureView FL_TextureViewCreate(WGPUTextureView handle);
void FL_TextureViewRelease(FL_TextureView* textureView);

#endif
