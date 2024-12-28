#include "texture_view.h"

namespace engine::wgpu
{
TextureView::~TextureView()
{
  wgpuTextureViewRelease(_handle);
}
}  // namespace engine::wgpu
