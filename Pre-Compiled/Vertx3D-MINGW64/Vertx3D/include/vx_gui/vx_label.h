#ifndef VX_LABEL_H
#define VX_LABEL_H

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

#include "../vertx3d.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vxLabel vxLabel;


VX_API vxLabel* vxNewLabel(const char* text, const char* fontPath, int fontSize, 
                           Vector3 position, float r, float g, float b);


VX_API void vxDestroyLabel(vxLabel* label);


VX_API void vxLabelSetText(vxLabel* label, const char* text);


VX_API void vxLabelSetPosition(vxLabel* label, Vector3 position);


VX_API void vxLabelSetColor(vxLabel* label, float r, float g, float b);


VX_API void vxLabelSetFontSize(vxLabel* label, int fontSize);


VX_API void vxDrawLabel(vxRenderContext* ctx, vxLabel* label);

#ifdef __cplusplus
}
#endif

#endif 