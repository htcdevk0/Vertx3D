#ifndef VX_CAMERA_H
#define VX_CAMERA_H

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

    typedef struct
    {
        Vector3 position;
        Vector3 target;
        Vector3 up;
        float fov;
        float aspect;
    } vxCamera;

    VX_API vxCamera *vxCreateCamera(Vector3 pos, Vector3 target, Vector3 up, float fov, float aspect);
    VX_API void vxUpdateCamera(vxCamera *cam);

#ifdef __cplusplus
}
#endif

#endif