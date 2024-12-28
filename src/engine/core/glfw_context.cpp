#include "glfw_context.h"

#include <iostream>

namespace engine::core
{
GLFWContext::GLFWContext()
{
  if (!glfwInit())
  {
    std::cerr << "[GLFW] Could not initialize GLFW" << std::endl;
  }
}

GLFWContext::~GLFWContext()
{
  glfwTerminate();
}

void GLFWContext::SetWindowHint(int hint, int value) const
{
  glfwWindowHint(hint, value);
}

GLFWwindow* GLFWContext::CreateWindowHandle(uint32_t width, uint32_t height,
                                            const char* title) const
{
  return glfwCreateWindow((int)width, (int)height, title, nullptr, nullptr);
}

void GLFWContext::DestroyWindowHandle(GLFWwindow* handle) const
{
  glfwDestroyWindow(handle);
}

void GLFWContext::PollEvents() const
{
  glfwPollEvents();
}

double GLFWContext::GetTime() const
{
  return glfwGetTime();
}

bool GLFWContext::WindowShouldClose(GLFWwindow* handle) const
{
  return glfwWindowShouldClose(handle);
}

}  // namespace engine::core
