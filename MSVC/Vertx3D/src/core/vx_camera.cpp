#include "../include/vx_core/vx_camera.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

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

#ifdef _WIN32
#define VERTX3D_BUILD_DLL
#endif

float camera_view_matrix[16] = {0};
float camera_proj_matrix[16] = {0};
int camera_active = 0;

static void lookAt(float *m, Vector3 eye, Vector3 target, Vector3 up)
{
    Vector3 f = {target.x - eye.x, target.y - eye.y, target.z - eye.z};
    float len = sqrtf(f.x * f.x + f.y * f.y + f.z * f.z);
    if (len > 0.0001f)
    {
        f.x /= len;
        f.y /= len;
        f.z /= len;
    }
    else
    {

        f.x = 0;
        f.y = 0;
        f.z = -1;
    }

    Vector3 up_corrected = up;
    if (fabsf(up.x) < 0.0001f && fabsf(up.y) < 0.0001f && fabsf(up.z) < 0.0001f)
    {
        up_corrected.x = 0;
        up_corrected.y = 1;
        up_corrected.z = 0;
    }

    Vector3 r = {up_corrected.y * f.z - up_corrected.z * f.y,
                 up_corrected.z * f.x - up_corrected.x * f.z,
                 up_corrected.x * f.y - up_corrected.y * f.x};
    len = sqrtf(r.x * r.x + r.y * r.y + r.z * r.z);
    if (len > 0.0001f)
    {
        r.x /= len;
        r.y /= len;
        r.z /= len;
    }
    else
    {

        r.x = 1;
        r.y = 0;
        r.z = 0;
    }

    Vector3 u = {f.y * r.z - f.z * r.y, f.z * r.x - f.x * r.z, f.x * r.y - f.y * r.x};

    m[0] = r.x;
    m[4] = r.y;
    m[8] = r.z;
    m[12] = -(r.x * eye.x + r.y * eye.y + r.z * eye.z);
    m[1] = u.x;
    m[5] = u.y;
    m[9] = u.z;
    m[13] = -(u.x * eye.x + u.y * eye.y + u.z * eye.z);
    m[2] = -f.x;
    m[6] = -f.y;
    m[10] = -f.z;
    m[14] = (f.x * eye.x + f.y * eye.y + f.z * eye.z);
    m[3] = 0;
    m[7] = 0;
    m[11] = 0;
    m[15] = 1;
}

static void perspective(float *m, float fov, float aspect, float near, float far)
{
    float f = 1.0f / tanf(fov * 0.5f * 3.14159f / 180.0f);
    memset(m, 0, sizeof(float) * 16);
    m[0] = f / aspect;
    m[5] = f;
    m[10] = (far + near) / (near - far);
    m[11] = -1.0f;
    m[14] = (2.0f * far * near) / (near - far);
}

VX_API vxCamera *vxCreateCamera(Vector3 pos, Vector3 target, Vector3 up, float fov, float aspect)
{
    vxCamera *cam = (vxCamera *)malloc(sizeof(vxCamera));
    cam->position = pos;
    cam->target = target;
    cam->up = up;
    cam->fov = fov;
    cam->aspect = aspect;
    return cam;
}

VX_API void vxUpdateCamera(vxCamera *cam)
{
    if (!cam)
        return;

    lookAt(camera_view_matrix, cam->position, cam->target, cam->up);
    perspective(camera_proj_matrix, cam->fov, cam->aspect, 0.1f, 1000.0f);
    camera_active = 1;
}