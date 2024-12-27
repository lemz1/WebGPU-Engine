#pragma once

#include <webgpu/webgpu.h>

#include <string>

#define StrToWGPU(str)                 \
  WGPUStringView                       \
  {                                    \
    .data = str, .length = strlen(str) \
  }

#define ViewToWGPU(view)                         \
  WGPUStringView                                 \
  {                                              \
    .data = view.data(), .length = view.length() \
  }

#define WGPUToString(view) std::string(view.data, view.length)
