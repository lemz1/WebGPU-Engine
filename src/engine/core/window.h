#ifndef FL_WINDOW_HEADER_DEFINE
#define FL_WINDOW_HEADER_DEFINE

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct
{
  GLFWwindow* handle;
  const char* title;
  uint32_t width;
  uint32_t height;
} FL_Window;

FL_Window FL_WindowCreate(uint32_t width, uint32_t height, const char* title);
void FL_WindowDestroy(FL_Window* window);

bool FL_WindowShouldClose(const FL_Window* window);

#endif
