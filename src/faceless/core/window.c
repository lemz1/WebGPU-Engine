#include "window.h"

#include <stdio.h>

FL_Window FL_WindowCreate(uint32_t width, uint32_t height, const char* title)
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* handle =
    glfwCreateWindow((int)width, (int)height, title, NULL, NULL);
  if (!handle)
  {
    perror("[GLFW] Could not create Window\n");
    return (FL_Window){0};
  }

  return (FL_Window){
    .handle = handle,
    .title = title,
    .width = width,
    .height = height,
  };
}

void FL_WindowDestroy(FL_Window* window)
{
  glfwDestroyWindow(window->handle);
}

bool FL_WindowShouldClose(const FL_Window* window)
{
  return glfwWindowShouldClose(window->handle);
}
