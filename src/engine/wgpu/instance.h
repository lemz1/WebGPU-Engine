#ifndef FL_INSTANCE_HEADER_DEFINE
#define FL_INSTANCE_HEADER_DEFINE

#include <webgpu/webgpu.h>

typedef struct
{
  WGPUInstance handle;
} FL_Instance;

FL_Instance FL_InstanceCreate();
void FL_InstanceRelease(FL_Instance* instance);

#endif
