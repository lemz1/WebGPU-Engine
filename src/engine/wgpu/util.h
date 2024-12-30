#ifndef FL_WGPU_UTIL_HEADER_DEFINE
#define FL_WGPU_UTIL_HEADER_DEFINE

#include <string.h>
#include <webgpu/webgpu.h>

#define FL_StrToWGPU(str)              \
  (WGPUStringView)                     \
  {                                    \
    .data = str, .length = strlen(str) \
  }

#endif
