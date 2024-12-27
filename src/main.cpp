#include <iostream>

#include "engine/wgpu/adapter.h"
#include "engine/wgpu/instance.h"
#include "engine/wgpu/surface.h"

int main()
{
  auto instance = engine::wgpu::Instance();
  auto adapter = engine::wgpu::Adapter(instance);

  WGPUAdapterInfo info{};
  wgpuAdapterGetInfo(adapter, &info);

  std::cout << "Vendor ID: " << info.vendorID << std::endl;

  return 0;
}
