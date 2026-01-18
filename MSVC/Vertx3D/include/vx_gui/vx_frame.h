#ifndef VX_FRAME_H
#define VX_FRAME_H

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

    typedef struct vxFrame vxFrame;

    VX_API vxFrame *vxNewFrame(float width, float height, Vector3 position, float r, float g, float b, float a);

    VX_API vxFrame *vxCreateFrame(float width, float height, float x, float y, float z,
                                  float r, float g, float b, float a);

    VX_API void vxDestroyFrame(vxFrame *frame);

    VX_API void vxFrameSetPosition(vxFrame *frame, Vector3 position);
    VX_API void vxFrameSetPositionF(vxFrame *frame, float x, float y, float z);

    VX_API void vxFrameSetSize(vxFrame *frame, float width, float height);

    VX_API void vxFrameSetColor(vxFrame *frame, float r, float g, float b, float a);

    VX_API void vxFrameSetVisible(vxFrame *frame, bool visible);

    VX_API void vxFrameSet2D(vxFrame *frame, bool is2D);

    VX_API void vxFrameSetBorder(vxFrame *frame, float thickness, float r, float g, float b, float a);

    VX_API void vxFrameSetCornerRadius(vxFrame *frame, float radius);

    VX_API void vxDrawFrame(vxRenderContext *ctx, vxFrame *frame);

#ifdef __cplusplus
}
#endif

#endif