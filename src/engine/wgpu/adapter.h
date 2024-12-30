#ifndef FL_ADAPTER_HEADER_DEFINE
#define FL_ADAPTER_HEADER_DEFINE

#include <webgpu/webgpu.h>

#include "engine/wgpu/instance.h"

typedef struct
{
  WGPUAdapter handle;
} FL_Adapter;

FL_Adapter FL_AdapterCreate(const FL_Instance* instance);
void FL_AdapterRelease(FL_Adapter* adapter);

#endif
