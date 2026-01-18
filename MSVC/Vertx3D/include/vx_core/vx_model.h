
#ifndef VX_MODEL_H
#define VX_MODEL_H

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

    typedef struct vxModel vxModel;

    VX_API vxModel *vxLoadModel(const char *filepath);

    VX_API vxModel *vxLoadModelWithTexture(const char *objFile, const char *textureFile);

    VX_API void vxDestroyModel(vxModel *model);

    VX_API void vxDrawModel(vxRenderContext *ctx, vxModel *model,
                            Vector3 position, float scale,
                            float rotationX, float rotationY, float rotationZ);

    VX_API void vxModelSetColor(vxModel *model, float r, float g, float b, float a);

    VX_API void vxModelSetWireframe(vxModel *model, bool wireframe);

    VX_API int vxModelGetTriangleCount(vxModel *model);

#ifdef __cplusplus
}
#endif

#endif