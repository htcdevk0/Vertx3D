#include "../../include/vx_core/vx_window.h"
#include "../../include/vx_core/vx_mesh.h"
#include "../include/vx_input/vx_mouse.h"

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

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <cstring>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_BMP
#include "../../thirdparty/stb/stb_image.h"

#if _WIN32
#include <windows.h>
#endif

struct vxWindow
{
    GLFWwindow *glfwWindow;
    int width;
    int height;
    int flags;
    double targetFrameTime;
    double lastFrameTime;

    unsigned char keys[348];
    unsigned char prevKeys[348];
    unsigned char mouseButtons[8];
    unsigned char prevMouseButtons[8];
    float mouseX, mouseY;
    float prevMouseX, prevMouseY;
    int mouseLocked;
};

struct vxRenderContext
{
    vxWindow *window;
    vxColor clearColor;
    int clearFlags;
};

static int glfw_initialized = 0;
static int glad_loaded = 0;

static void keyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods)
{
    vxWindow *window = (vxWindow *)glfwGetWindowUserPointer(glfwWindow);
    if (!window || key < 0 || key >= 348)
        return;

    window->keys[key] = (action != GLFW_RELEASE);
}

static void mouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int mods)
{
    vxWindow *window = (vxWindow *)glfwGetWindowUserPointer(glfwWindow);
    if (!window || button < 0 || button >= 8)
        return;

    window->mouseButtons[button] = (action != GLFW_RELEASE);
}

static void cursorPosCallback(GLFWwindow *glfwWindow, double xpos, double ypos)
{
    vxWindow *window = (vxWindow *)glfwGetWindowUserPointer(glfwWindow);
    if (!window)
        return;

    window->mouseX = (float)xpos;
    window->mouseY = (float)ypos;
}

VX_API int vxInit(int flags)
{
    if (glfw_initialized)
        return 1;

    if (!glfwInit())
    {
        std::cerr << "GLFW init failed!" << std::endl;
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfw_initialized = 1;
    return 1;
}

void vxTerminate(void)
{
    if (glfw_initialized)
    {
        glfwTerminate();
        glfw_initialized = 0;
        glad_loaded = 0;
    }
}

VX_API vxWindow *vxCreateWindow(int width, int height, const char *title)
{
    if (!glfw_initialized)
    {
        std::cerr << "GLFW not initialized! Call vxInit() first!" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    vxWindow *window = new vxWindow();
    window->width = width;
    window->height = height;

    window->glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window->glfwWindow)
    {
        delete window;
        std::cerr << "Failed to create window!" << std::endl;
        return nullptr;
    }

    glfwMakeContextCurrent(window->glfwWindow);
    vxInitMouse(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        glfwDestroyWindow(window->glfwWindow);
        delete window;
        return nullptr;
    }

    glfwSwapInterval(0);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    memset(window->keys, 0, sizeof(window->keys));
    memset(window->prevKeys, 0, sizeof(window->prevKeys));
    memset(window->mouseButtons, 0, sizeof(window->mouseButtons));
    memset(window->prevMouseButtons, 0, sizeof(window->prevMouseButtons));
    window->mouseX = window->mouseY = 0.0f;
    window->prevMouseX = window->prevMouseY = 0.0f;

    window->flags = 0;
    window->flags |= VX_WINDOW_RESIZABLE;
    window->flags |= VX_WINDOW_DECORATED;
    window->flags |= VX_WINDOW_VISIBLE;
    window->flags |= VX_WINDOW_AUTO_ICONIFY;
    window->flags |= VX_WINDOW_FOCUS_ON_SHOW;

    glfwSetFramebufferSizeCallback(window->glfwWindow, [](GLFWwindow *glfwWin, int w, int h)
                                   {
                                       glViewport(0, 0, w, h);   
                                       vxUpdateWindowSize(w, h); });

    glfwSetKeyCallback(window->glfwWindow, keyCallback);
    glfwSetMouseButtonCallback(window->glfwWindow, mouseButtonCallback);
    glfwSetCursorPosCallback(window->glfwWindow, cursorPosCallback);

    glfwShowWindow(window->glfwWindow);

    return window;
}

int vxWindowShouldClose(vxWindow *window)
{
    if (!window || !window->glfwWindow)
        return 1;
    return glfwWindowShouldClose(window->glfwWindow);
}

void vxPollEvents(vxWindow *window)
{
    if (!window)
        return;

    memcpy(window->prevKeys, window->keys, sizeof(window->keys));
    memcpy(window->prevMouseButtons, window->mouseButtons, sizeof(window->mouseButtons));
    window->prevMouseX = window->mouseX;
    window->prevMouseY = window->mouseY;

    for (int i = 0; i < 348; i++)
    {
        int state = glfwGetKey(window->glfwWindow, i);
        window->keys[i] = (state == GLFW_PRESS) ? 1 : 0;
    }

    if (window->mouseLocked && window->glfwWindow)
    {
        int width, height;
        glfwGetWindowSize(window->glfwWindow, &width, &height);

        float centerX = width / 2.0f;
        float centerY = height / 2.0f;

        glfwSetCursorPos(window->glfwWindow, centerX, centerY);
        window->mouseX = centerX;
        window->mouseY = centerY;
    }

    glfwPollEvents();
    vxUpdateMouse();
}

void vxSwapBuffers(vxWindow *window)
{
    if (window && window->glfwWindow)
    {
        glfwSwapBuffers(window->glfwWindow);
    }
}

void vxDestroyWindow(vxWindow *window)
{
    if (window)
    {
        if (window->glfwWindow)
        {
            glfwDestroyWindow(window->glfwWindow);
        }
        delete window;
    }
}

int vxKeyPressed(vxWindow *window, int key)
{
    if (!window || key < 0 || key >= 348)
        return VX_KEY_RELEASED;

    if (window->keys[key] && !window->prevKeys[key])
    {
        return VX_KEY_PRESSED;
    }

    return VX_KEY_RELEASED;
}

VX_API int vxKeyDown(vxWindow *window, int key)
{
    if (!window || key < 0 || key >= 348)
        return 0;
    return window->keys[key] ? 1 : 0;
}

int vxKeyReleased(vxWindow *window, int key)
{
    if (!window || key < 0 || key >= 348)
        return 0;

    if (!window->keys[key] && window->prevKeys[key])
    {
        return 1;
    }

    return 0;
}

void vxGetMousePosition(vxWindow *window, float *x, float *y)
{
    if (!window)
    {
        if (x)
            *x = 0;
        if (y)
            *y = 0;
        return;
    }

    if (x)
        *x = window->mouseX;
    if (y)
        *y = window->mouseY;
}

void vxGetMouseDelta(vxWindow *window, float *dx, float *dy)
{
    if (!window)
    {
        if (dx)
            *dx = 0;
        if (dy)
            *dy = 0;
        return;
    }

    if (dx)
        *dx = window->mouseX - window->prevMouseX;
    if (dy)
        *dy = window->mouseY - window->prevMouseY;
}

int vxMouseButtonPressed(vxWindow *window, int button)
{
    if (!window || button < 0 || button >= 8)
        return 0;

    if (window->mouseButtons[button] && !window->prevMouseButtons[button])
    {
        return 1;
    }

    return 0;
}

int vxMouseButtonDown(vxWindow *window, int button)
{
    if (!window || button < 0 || button >= 8)
        return 0;
    return window->mouseButtons[button] ? 1 : 0;
}

void vxSetMouseLock(vxWindow *window, int lock)
{
    if (!window || !window->glfwWindow)
        return;

    window->mouseLocked = lock ? 1 : 0;

    if (lock)
    {
        glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

int vxGetMouseLock(vxWindow *window)
{
    if (!window)
        return 0;
    return window->mouseLocked;
}

vxRenderContext *vxCreateRenderContext(vxWindow *window)
{
    if (!window)
        return nullptr;

    vxRenderContext *ctx = new vxRenderContext();
    ctx->window = window;
    ctx->clearColor = VX_BLACK;
    ctx->clearFlags = VX_CLEAR_COLOR | VX_CLEAR_DEPTH;

    return ctx;
}

void vxDestroyRenderContext(vxRenderContext *ctx)
{
    if (ctx)
    {
        delete ctx;
    }
}

void vxClearColor(vxRenderContext *ctx, float r, float g, float b)
{
    if (!ctx)
        return;
    ctx->clearColor.r = r;
    ctx->clearColor.g = g;
    ctx->clearColor.b = b;
    ctx->clearColor.a = 1.0f;
}

void vxClear(vxRenderContext *ctx, int flags)
{
    if (!ctx || !ctx->window)
        return;

    glClearColor(ctx->clearColor.r, ctx->clearColor.g, ctx->clearColor.b, ctx->clearColor.a);

    GLbitfield glFlags = 0;
    if (flags & VX_CLEAR_COLOR)
        glFlags |= GL_COLOR_BUFFER_BIT;
    if (flags & VX_CLEAR_DEPTH)
        glFlags |= GL_DEPTH_BUFFER_BIT;

    glClear(glFlags);
}

VX_API void vxExitAndClean(vxWindow *window)
{
    if (!window)
        return;

    glfwSetWindowShouldClose(window->glfwWindow, 1);

    glfwPollEvents();

    glfwSwapBuffers(window->glfwWindow);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window->glfwWindow);
}

VX_API void vxDestroyAll(vxWindow *window, vxRenderContext *context, int count, vxMesh **objects)
{
    if (!objects)
        return;

    for (int i = 0; i < count; i++)
    {
        if (objects[i])
        {
            vxDestroyMesh(objects[i]);
            objects[i] = nullptr;
        }
    }
}

VX_API void vxSetWindowIcon(vxWindow *window, const char *iconPath)
{
    if (!window || !window->glfwWindow || !iconPath)
        return;

    int width, height, channels;
    unsigned char *data = stbi_load(iconPath, &width, &height, &channels, 4);

    if (!data)
    {
        std::cerr << "ERROR: Could not load the icon: " << iconPath << std::endl;
        std::cerr << "Error: " << stbi_failure_reason() << std::endl;
        return;
    }

    GLFWimage icon;
    icon.width = width;
    icon.height = height;
    icon.pixels = data;

    glfwSetWindowIcon(window->glfwWindow, 1, &icon);

    stbi_image_free(data);
}

VX_API void vxSetWindowFlags(vxWindow *window, int flags, int value)
{
    if (!window || !window->glfwWindow)
        return;

    if (flags & VX_WINDOW_RESIZABLE)
    {
        glfwSetWindowAttrib(window->glfwWindow, GLFW_RESIZABLE, value ? GLFW_TRUE : GLFW_FALSE);
    }
    if (flags & VX_WINDOW_DECORATED)
    {
        glfwSetWindowAttrib(window->glfwWindow, GLFW_DECORATED, value ? GLFW_TRUE : GLFW_FALSE);
    }
    if (flags & VX_WINDOW_FLOATING)
    {
        glfwSetWindowAttrib(window->glfwWindow, GLFW_FLOATING, value ? GLFW_TRUE : GLFW_FALSE);
    }
    if (flags & VX_WINDOW_VISIBLE)
    {
        if (value)
        {
            glfwShowWindow(window->glfwWindow);
        }
        else
        {
            glfwHideWindow(window->glfwWindow);
        }
    }
    if (flags & VX_WINDOW_AUTO_ICONIFY)
    {
        glfwSetWindowAttrib(window->glfwWindow, GLFW_AUTO_ICONIFY, value ? GLFW_TRUE : GLFW_FALSE);
    }
    if (flags & VX_WINDOW_MAXIMIZED)
    {
        if (value)
        {
            glfwMaximizeWindow(window->glfwWindow);
        }
        else
        {
            glfwRestoreWindow(window->glfwWindow);
        }
    }
    if (flags & VX_WINDOW_VSYNC)
    {
        glfwSwapInterval(value ? 1 : 0);
    }
    if (flags & VX_WINDOW_CENTER_CURSOR)
    {
        glfwSetInputMode(window->glfwWindow, GLFW_CURSOR,
                         value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
    if (flags & VX_WINDOW_FOCUS_ON_SHOW)
    {
        glfwSetWindowAttrib(window->glfwWindow, GLFW_FOCUS_ON_SHOW, value ? GLFW_TRUE : GLFW_FALSE);
    }
    if (flags & VX_WINDOW_FULLSCREEN_BORDERLESS)
    {
        if (value)
        {

            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            if (monitor)
            {
                const GLFWvidmode *mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(window->glfwWindow, monitor, 0, 0,
                                     mode->width, mode->height, mode->refreshRate);
                window->width = mode->width;
                window->height = mode->height;
                glViewport(0, 0, mode->width, mode->height);
            }
        }
        else
        {

            glfwSetWindowMonitor(window->glfwWindow, NULL, 100, 100,
                                 window->width, window->height, GLFW_DONT_CARE);
        }
    }
    if (flags & VX_WINDOW_CURSOR_DISABLED)
    {
        if (value)
        {
            glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (value)
    {
        window->flags |= flags;
    }
    else
    {
        window->flags &= ~flags;
    }
}

VX_API void vxGetWindowFlags(vxWindow *window, int *flags)
{
    if (window && flags)
    {
        *flags = window->flags;
    }
}

VX_API void vxSetWindowTitle(vxWindow *window, const char *title)
{
    if (window && window->glfwWindow && title)
    {
        glfwSetWindowTitle(window->glfwWindow, title);
    }
}

VX_API void vxSetWindowSize(vxWindow *window, int width, int height)
{
    if (window && window->glfwWindow)
    {
        glfwSetWindowSize(window->glfwWindow, width, height);
        window->width = width;
        window->height = height;
        glViewport(0, 0, width, height);
    }
}

VX_API void vxSetWindowPos(vxWindow *window, int x, int y)
{
    if (window && window->glfwWindow)
    {
        glfwSetWindowPos(window->glfwWindow, x, y);
    }
}

VX_API void vxSetTargetFPS(vxWindow *window, int fps)
{
    if (!window || fps <= 0)
        return;

    window->targetFrameTime = 1.0 / fps;
    window->flags |= VX_WINDOW_TARGET_FPS;
}

VX_API void vxLimitFPS(vxWindow *window)
{
    if (!window)
        return;

    if (window->flags & VX_WINDOW_TARGET_FPS)
    {
        double currentTime = glfwGetTime();
        double elapsed = currentTime - window->lastFrameTime;

        if (elapsed < window->targetFrameTime)
        {
            double sleepTime = window->targetFrameTime - elapsed;

            int sleepMs = (int)(sleepTime * 1000.0);
            if (sleepMs > 0)
            {

#ifdef _WIN32
                Sleep(sleepMs);
#else
                usleep(sleepMs * 1000);
#endif
            }

            do
            {
                currentTime = glfwGetTime();
                elapsed = currentTime - window->lastFrameTime;
            } while (elapsed < window->targetFrameTime);
        }

        window->lastFrameTime = currentTime;
    }
    else
    {

        window->lastFrameTime = glfwGetTime();
    }
}

VX_API double vxGetCurrentFPS(vxWindow *window)
{
    if (!window)
        return 0.0;

    static double lastTime = glfwGetTime();
    static int frameCount = 0;
    static double fps = 0.0;

    double currentTime = glfwGetTime();
    frameCount++;

    if (currentTime - lastTime >= 1.0)
    {
        fps = frameCount / (currentTime - lastTime);
        frameCount = 0;
        lastTime = currentTime;
    }

    return fps;
}

VX_API void vxSetWindowFullscreen(vxWindow *window, int fullscreen, int borderless)
{
    if (!window || !window->glfwWindow)
        return;

    if (fullscreen)
    {
        if (borderless)
        {

            vxSetWindowFlags(window, VX_WINDOW_FULLSCREEN_BORDERLESS, 1);
            vxSetWindowFlags(window, VX_WINDOW_DECORATED, 0);
            vxSetWindowFlags(window, VX_WINDOW_RESIZABLE, 0);
        }
        else
        {

            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            if (monitor)
            {
                const GLFWvidmode *mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(window->glfwWindow, monitor, 0, 0,
                                     mode->width, mode->height, mode->refreshRate);
            }
        }
    }
    else
    {

        glfwSetWindowMonitor(window->glfwWindow, NULL, 100, 100,
                             window->width, window->height, GLFW_DONT_CARE);
        vxSetWindowFlags(window, VX_WINDOW_DECORATED, 1);
        vxSetWindowFlags(window, VX_WINDOW_RESIZABLE, 1);
    }
}

VX_API int vxGetWindowWidth(vxWindow *window)
{
    if (!window)
        return 0;
    return window->width;
}

VX_API int vxGetWindowHeight(vxWindow *window)
{
    if (!window)
        return 0;
    return window->height;
}

VX_API GLFWwindow *vxGetGLFWWindow(vxWindow *window)
{
    if (!window)
        return NULL;
    return window->glfwWindow;
}
