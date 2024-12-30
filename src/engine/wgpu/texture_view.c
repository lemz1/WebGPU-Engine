#include "texture_view.h"

FL_TextureView FL_TextureViewCreate(WGPUTextureView handle)
{
  return (FL_TextureView){.handle = handle};
}

void FL_TextureViewRelease(FL_TextureView* textureView)
{
  wgpuTextureViewRelease(textureView->handle);
}
