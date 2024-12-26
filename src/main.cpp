#include <webgpu/webgpu_cpp.h>

#include <cstdlib>
#include <iostream>

int main()
{
  wgpu::InstanceDescriptor instanceDescriptor{};
  instanceDescriptor.features.timedWaitAnyEnable = true;
  wgpu::Instance instance = wgpu::CreateInstance(&instanceDescriptor);
  if (instance == nullptr)
  {
    std::cerr << "Instance creation failed!\n";
    return 1;
  }
  // Synchronously request the adapter.
  wgpu::RequestAdapterOptions options = {};
  wgpu::Adapter adapter;
  wgpu::RequestAdapterCallbackInfo callbackInfo = {};
  callbackInfo.nextInChain = nullptr;
  callbackInfo.mode = wgpu::CallbackMode::WaitAnyOnly;
  callbackInfo.callback = [](WGPURequestAdapterStatus status,
                             WGPUAdapter adapter, WGPUStringView message,
                             void *userdata)
  {
    if (status != WGPURequestAdapterStatus_Success)
    {
      std::cerr << "Failed to get an adapter:" << message.data;
      return;
    }
    *static_cast<wgpu::Adapter *>(userdata) = wgpu::Adapter::Acquire(adapter);
  };
  callbackInfo.userdata = &adapter;
  instance.WaitAny(instance.RequestAdapter(&options, callbackInfo), UINT64_MAX);
  if (adapter == nullptr)
  {
    std::cerr << "RequestAdapter failed!\n";
    return 1;
  }

  wgpu::DawnAdapterPropertiesPowerPreference power_props{};

  wgpu::AdapterInfo info{};
  info.nextInChain = &power_props;

  adapter.GetInfo(&info);

  return 0;
}
