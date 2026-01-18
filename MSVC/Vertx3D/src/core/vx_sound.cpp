
#include "../include/vx_core/vx_sound.h"
#include <windows.h>
#include <mmsystem.h>

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
#pragma comment(lib, "winmm.lib")
#endif

struct vxSound
{
    char filepath[260];
    int isPlaying;
    vxSoundMode mode;
};

static int soundInitialized = 0;
static vxSound sounds[32];
static int soundCount = 0;

VX_API int vxInitSound()
{
    if (soundInitialized)
        return 1;

    soundInitialized = 1;
    return 1;
}

VX_API void vxPlaySound(const char *filepath, vxSoundMode mode)
{
    if (!soundInitialized)
        return;

    DWORD attrs = GetFileAttributesA(filepath);
    if (attrs == INVALID_FILE_ATTRIBUTES)
    {
        return;
    }

    DWORD flags = SND_FILENAME | SND_ASYNC;

    if (mode == VX_SOUND_REPEAT)
    {
        flags |= SND_LOOP;
    }

    PlaySoundA(filepath, NULL, flags);

    if (soundCount < 32)
    {
        strncpy(sounds[soundCount].filepath, filepath, 259);
        sounds[soundCount].isPlaying = 1;
        sounds[soundCount].mode = mode;
        soundCount++;
    }
}

VX_API void vxStopAllSounds()
{

    PlaySound(NULL, NULL, 0);

    for (int i = 0; i < soundCount; i++)
    {
        sounds[i].isPlaying = 0;
    }
}

VX_API void vxCloseSound()
{
    vxStopAllSounds();
    soundInitialized = 0;
    soundCount = 0;
}