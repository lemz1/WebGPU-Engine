#pragma once

#include <GLFW/glfw3.h>

#include "glfw_context.h"

namespace engine::core
{
class Window
{
 public:
  explicit Window(const GLFWContext& context, uint32_t width, uint32_t height,
                  const char* title);
  ~Window();

  operator GLFWwindow*() const
  {
    return _handle;
  }

  bool ShouldClose();

 private:
  const GLFWContext& _context;
  GLFWwindow* _handle;
  uint32_t _width;
  uint32_t _height;
};
}  // namespace engine::core
