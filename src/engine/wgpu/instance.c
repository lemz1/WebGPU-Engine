#include "instance.h"

#include <stdbool.h>
#include <stdio.h>

FL_Instance FL_InstanceCreate()
{
  WGPUInstanceDescriptor descriptor = {0};
  WGPUInstance handle = wgpuCreateInstance(&descriptor);
  if (!handle)
  {
    perror("[WebGPU] Could not create Instance\n");
    return (FL_Instance){0};
  }
  return (FL_Instance){.handle = handle};
}

void FL_InstanceRelease(FL_Instance* instance)
{
  wgpuInstanceRelease(instance->handle);
}
