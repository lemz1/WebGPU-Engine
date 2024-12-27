#pragma once

#include <webgpu/webgpu.h>

#include <string>

#define WGPUStringViewInit(str)        \
  WGPUStringView                       \
  {                                    \
    .data = str, .length = strlen(str) \
  }

#define WGPUString(view) std::string(view.data, view.length)
