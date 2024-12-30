#include "adapter.h"

#include <stdbool.h>
#include <stdio.h>

#include "engine/wgpu/util.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef struct
{
  WGPUAdapter adapter;
  bool finished;
} FL_AdapterUserData;

static void RequestAdapater(WGPURequestAdapterStatus status,
                            WGPUAdapter adapter, WGPUStringView message,
                            void* userdata);

FL_Adapter FL_AdapterCreate(const FL_Instance* instance)
{
  FL_AdapterUserData data = {
    .adapter = NULL,
    .finished = false,
  };
  WGPURequestAdapterOptions options = {0};
  wgpuInstanceRequestAdapter(instance->handle, &options, RequestAdapater,
                             &data);

#ifdef __EMSCRIPTEN__
  while (!data.finished)
  {
    emscripten_sleep(100);
  }
#endif

  if (!data.adapter)
  {
    perror("[WebGPU] Failed to request adapter!\n");
    return (FL_Adapter){0};
  }

  WGPUAdapter handle = data.adapter;

  return (FL_Adapter){.handle = handle};
}

void FL_AdapterRelease(FL_Adapter* adapter)
{
  wgpuAdapterRelease(adapter->handle);
}

static void RequestAdapater(WGPURequestAdapterStatus status,
                            WGPUAdapter adapter, WGPUStringView message,
                            void* userdata)
{
  FL_AdapterUserData* data = (FL_AdapterUserData*)userdata;
  data->finished = true;
  if (status != WGPURequestAdapterStatus_Success)
  {
    fprintf(stderr, "[WebGPU] Failed to get an adapter: %.*s\n",
            (int)message.length, message.data);
    return;
  }
  data->adapter = adapter;
}
