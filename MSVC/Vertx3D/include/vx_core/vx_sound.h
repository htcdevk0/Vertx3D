#ifndef VX_SOUND_H
#define VX_SOUND_H

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
        VX_SOUND_PLAY_ONCE = 0,
        VX_SOUND_REPEAT
    } vxSoundMode;

    VX_API int vxInitSound();
    VX_API void vxPlaySound(const char *filepath, vxSoundMode mode);
    VX_API void vxStopAllSounds();
    VX_API void vxCloseSound();

#ifdef __cplusplus
}
#endif

#endif