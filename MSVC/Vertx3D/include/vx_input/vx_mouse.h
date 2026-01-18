
#ifndef VX_MOUSE_H
#define VX_MOUSE_H

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

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        VX_MOUSE_RELEASED = 0,
        VX_MOUSE_PRESSED,
        VX_MOUSE_CLICKED
    } vxMouseState;

    typedef enum
    {
        VX_MOUSE_LEFT = 0,
        VX_MOUSE_RIGHT,
        VX_MOUSE_MIDDLE,
        VX_MOUSE_BUTTON_4,
        VX_MOUSE_BUTTON_5
    } vxMouseButton;

    VX_API float vxGetMouseX(vxWindow *window);
    VX_API float vxGetMouseY(vxWindow *window);
    VX_API void vxGetMousePos(vxWindow *window, float *x, float *y);

    VX_API int vxMouseDown(vxWindow *window, vxMouseButton button);
    VX_API int vxMousePressed(vxWindow *window, vxMouseButton button);
    VX_API int vxMouseReleased(vxWindow *window, vxMouseButton button);

    VX_API void vxSetMouseLock(vxWindow *window, int lock);
    VX_API int vxGetMouseLock(vxWindow *window);

    VX_API void vxInitMouse(vxWindow *window);
    VX_API void vxUpdateMouse();

#ifdef __cplusplus
}
#endif

#endif