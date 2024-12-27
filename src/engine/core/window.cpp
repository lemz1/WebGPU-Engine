#include "window.h"

#include <iostream>

namespace engine::core
{
Window::Window(const GLFWContext& context, uint32_t width, uint32_t height,
               const char* title)
    : _context(context), _width(width), _height(height), _title(title)
{
  context.SetWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  _handle = context.CreateWindowHandle(width, height, title);
  if (!_handle)
  {
    std::cerr << "[GLFW] Could not create Window" << std::endl;
    return;
  }
}

Window::~Window()
{
  _context.DestroyWindowHandle(_handle);
}

bool Window::ShouldClose()
{
  return _context.WindowShouldClose(_handle);
}
}  // namespace engine::core
