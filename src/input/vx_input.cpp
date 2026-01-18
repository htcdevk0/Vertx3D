#include "../../include/vx_input/vx_input.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstring>

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

struct InputState
{

    unsigned char keys[348];
    unsigned char prevKeys[348];

    float mouseX, mouseY;
    float prevMouseX, prevMouseY;
    unsigned char mouseButtons[8];
    unsigned char prevMouseButtons[8];

    int mouseLocked;
    float centerX, centerY;
};

static int convertKeyCode(int vxKey)
{

    return vxKey;
}

static InputState *getInputState(vxWindow *window)
{

    return (InputState *)glfwGetWindowUserPointer((GLFWwindow *)window);
}

static void keyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods)
{
    InputState *state = (InputState *)glfwGetWindowUserPointer(glfwWindow);
    if (!state || key < 0 || key >= 348)
        return;

    state->keys[key] = (action != GLFW_RELEASE);
}

static void mouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int mods)
{
    InputState *state = (InputState *)glfwGetWindowUserPointer(glfwWindow);
    if (!state || button < 0 || button >= 8)
        return;

    state->mouseButtons[button] = (action != GLFW_RELEASE);
}

static void cursorPosCallback(GLFWwindow *glfwWindow, double xpos, double ypos)
{
    InputState *state = (InputState *)glfwGetWindowUserPointer(glfwWindow);
    if (!state)
        return;

    state->mouseX = (float)xpos;
    state->mouseY = (float)ypos;
}

void initInputForWindow(vxWindow *window)
{
    GLFWwindow *glfwWindow = (GLFWwindow *)window;

    InputState *state = new InputState();
    memset(state, 0, sizeof(InputState));

    glfwSetWindowUserPointer(glfwWindow, state);

    glfwSetKeyCallback(glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
    glfwSetCursorPosCallback(glfwWindow, cursorPosCallback);

    double x, y;
    glfwGetCursorPos(glfwWindow, &x, &y);
    state->mouseX = state->prevMouseX = (float)x;
    state->mouseY = state->prevMouseY = (float)y;
}

void updateInput(vxWindow *window)
{
    InputState *state = getInputState(window);
    if (!state)
        return;

    memcpy(state->prevKeys, state->keys, sizeof(state->keys));
    memcpy(state->prevMouseButtons, state->mouseButtons, sizeof(state->mouseButtons));

    state->prevMouseX = state->mouseX;
    state->prevMouseY = state->mouseY;

    if (state->mouseLocked)
    {
        GLFWwindow *glfwWindow = (GLFWwindow *)window;
        int width, height;
        glfwGetWindowSize(glfwWindow, &width, &height);

        state->centerX = width / 2.0f;
        state->centerY = height / 2.0f;

        glfwSetCursorPos(glfwWindow, state->centerX, state->centerY);
        state->mouseX = state->centerX;
        state->mouseY = state->centerY;
    }
}

VX_API int vxKeyPressed(vxWindow *window, int key)
{
    InputState *state = getInputState(window);
    if (!state)
        return VX_KEY_RELEASED;

    int glfwKey = convertKeyCode(key);
    if (glfwKey < 0 || glfwKey >= 348)
        return VX_KEY_RELEASED;

    if (state->keys[glfwKey] && !state->prevKeys[glfwKey])
    {
        return VX_KEY_PRESSED;
    }

    return VX_KEY_RELEASED;
}

VX_API int vxKeyDown(vxWindow *window, int key)
{
    InputState *state = getInputState(window);
    if (!state)
        return 0;

    int glfwKey = convertKeyCode(key);
    if (glfwKey < 0 || glfwKey >= 348)
        return 0;

    return state->keys[glfwKey] ? 1 : 0;
}

VX_API int vxKeyReleased(vxWindow *window, int key)
{
    InputState *state = getInputState(window);
    if (!state)
        return 0;

    int glfwKey = convertKeyCode(key);
    if (glfwKey < 0 || glfwKey >= 348)
        return 0;

    if (!state->keys[glfwKey] && state->prevKeys[glfwKey])
    {
        return 1;
    }

    return 0;
}

VX_API void vxGetMousePosition(vxWindow *window, float *x, float *y)
{
    InputState *state = getInputState(window);
    if (!state)
    {
        if (x)
            *x = 0;
        if (y)
            *y = 0;
        return;
    }

    if (x)
        *x = state->mouseX;
    if (y)
        *y = state->mouseY;
}

VX_API void vxGetMouseDelta(vxWindow *window, float *dx, float *dy)
{
    InputState *state = getInputState(window);
    if (!state)
    {
        if (dx)
            *dx = 0;
        if (dy)
            *dy = 0;
        return;
    }

    if (dx)
        *dx = state->mouseX - state->prevMouseX;
    if (dy)
        *dy = state->mouseY - state->prevMouseY;
}

VX_API int vxMouseButtonPressed(vxWindow *window, int button)
{
    InputState *state = getInputState(window);
    if (!state || button < 0 || button >= 8)
        return 0;

    if (state->mouseButtons[button] && !state->prevMouseButtons[button])
    {
        return 1;
    }

    return 0;
}

VX_API int vxMouseButtonDown(vxWindow *window, int button)
{
    InputState *state = getInputState(window);
    if (!state || button < 0 || button >= 8)
        return 0;

    return state->mouseButtons[button] ? 1 : 0;
}

VX_API void vxSetMouseLock(vxWindow *window, int lock)
{
    InputState *state = getInputState(window);
    if (!state)
        return;

    state->mouseLocked = lock ? 1 : 0;
    GLFWwindow *glfwWindow = (GLFWwindow *)window;

    if (lock)
    {
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

VX_API int vxGetMouseLock(vxWindow *window)
{
    InputState *state = getInputState(window);
    if (!state)
        return 0;

    return state->mouseLocked;
}