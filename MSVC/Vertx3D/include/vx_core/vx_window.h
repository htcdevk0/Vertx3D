#ifndef VX_WINDOW_H
#define VX_WINDOW_H

/*

Made by: htcdevk0
Access: https://vertx3d.sbs to more information

Version:
Pre-Alpha

Version Type:
For user : No Comments

Compiler:
MSVC - Visual Studio 2022

*/

#include "../vertx3d.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

VX_API int vxGetWindowWidth(vxWindow *window);
VX_API int vxGetWindowHeight(vxWindow *window);

VX_API GLFWwindow *vxGetGLFWWindow(vxWindow *window);

#endif