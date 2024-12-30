#include "instance.h"

#include <stdbool.h>

FL_Instance FL_InstanceCreate()
{
  WGPUInstanceDescriptor descriptor = {0};
  WGPUInstance handle = wgpuCreateInstance(&descriptor);
  return (FL_Instance){.handle = handle};
}

void FL_InstanceRelease(FL_Instance* instance)
{
  wgpuInstanceRelease(instance->handle);
}
