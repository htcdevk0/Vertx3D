
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/vx_input/vx_mouse.h"
#include "../include/vx_core/vx_window.h"

/*

Made by: htcdevk0
Access: https://vertx3d.sbs to more information

Version:
Pre-Alpha

Version Type:
For user : No Comments

Compiler:
Mingw64 - Msys2

*/

#ifdef _WIN32
#define VERTX3D_BUILD_DLL
#endif

static int mouse_current[5] = {0};
static int mouse_previous[5] = {0};
static GLFWwindow *current_glfw_window = NULL;

static void mouse_button_callback(GLFWwindow *glfwWindow, int button, int action, int mods)
{
    (void)glfwWindow;
    (void)mods;

    int vxButton = -1;
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        vxButton = VX_MOUSE_LEFT;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        vxButton = VX_MOUSE_RIGHT;
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        vxButton = VX_MOUSE_MIDDLE;
    else
        return;

    if (action == GLFW_PRESS)
        mouse_current[vxButton] = 1;
    else if (action == GLFW_RELEASE)
        mouse_current[vxButton] = 0;
}

VX_API void vxInitMouse(vxWindow *window)
{
    if (!window)
        return;

    current_glfw_window = vxGetGLFWWindow(window);

    if (current_glfw_window)
    {

        glfwSetMouseButtonCallback(current_glfw_window, mouse_button_callback);

        for (int i = 0; i < 5; i++)
        {
            int state = glfwGetMouseButton(current_glfw_window, i);
            mouse_current[i] = (state == GLFW_PRESS) ? 1 : 0;
            mouse_previous[i] = mouse_current[i];
        }
    }
}

VX_API void vxUpdateMouse()
{
    if (!current_glfw_window)
        return;

    for (int i = 0; i < 5; i++)
        mouse_previous[i] = mouse_current[i];

    for (int i = 0; i < 5; i++)
    {
        int state = glfwGetMouseButton(current_glfw_window, i);
        mouse_current[i] = (state == GLFW_PRESS) ? 1 : 0;
    }
}

VX_API float vxGetMouseX(vxWindow *window)
{
    (void)window;
    if (!current_glfw_window)
        return 0;

    double x, y;
    glfwGetCursorPos(current_glfw_window, &x, &y);
    return (float)x;
}

VX_API float vxGetMouseY(vxWindow *window)
{
    (void)window;
    if (!current_glfw_window)
        return 0;

    double x, y;
    glfwGetCursorPos(current_glfw_window, &x, &y);
    return (float)y;
}

VX_API void vxGetMousePos(vxWindow *window, float *x, float *y)
{
    (void)window;
    if (!current_glfw_window || !x || !y)
        return;

    double dx, dy;
    glfwGetCursorPos(current_glfw_window, &dx, &dy);
    *x = (float)dx;
    *y = (float)dy;
}

VX_API int vxMouseDown(vxWindow *window, vxMouseButton button)
{
    (void)window;
    if (button < 0 || button >= 5)
        return 0;
    return mouse_current[button] == 1;
}

VX_API int vxMousePressed(vxWindow *window, vxMouseButton button)
{
    (void)window;
    if (button < 0 || button >= 5)
        return 0;
    return mouse_current[button] == 1 && mouse_previous[button] == 0;
}

VX_API int vxMouseReleased(vxWindow *window, vxMouseButton button)
{
    (void)window;
    if (button < 0 || button >= 5)
        return 0;
    return mouse_current[button] == 0 && mouse_previous[button] == 1;
}
