#pragma once

#include <string.h>
#include <webgpu/webgpu.h>

#define WGPUStringViewInit(str)        \
  WGPUStringView                       \
  {                                    \
    .data = str, .length = strlen(str) \
  }
