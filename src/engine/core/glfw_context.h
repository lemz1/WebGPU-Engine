#pragma once

#include <GLFW/glfw3.h>

namespace engine::core
{
class GLFWContext
{
 public:
  explicit GLFWContext();
  ~GLFWContext();

  void SetWindowHint(int hint, int value) const;

  GLFWwindow* CreateWindowHandle(uint32_t width, uint32_t height,
                                 const char* title) const;

  void DestroyWindowHandle(GLFWwindow* handle) const;

  void PollEvents() const;

  double GetTime() const;

  bool WindowShouldClose(GLFWwindow* handle) const;
};
}  // namespace engine::core
