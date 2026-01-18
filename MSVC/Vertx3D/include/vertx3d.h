#ifndef VERTX3D_H
#define VERTX3D_H

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
#ifdef VERTX3D_BUILD_DLL
#define VX_API __declspec(dllexport)
#else
#define VX_API __declspec(dllimport)
#endif
#else
#define VX_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct vxWindow vxWindow;
    typedef struct vxRenderContext vxRenderContext;
    typedef struct vxMesh vxMesh;

    typedef struct
    {
        float x, y, z;
    } Vector3;

#ifdef __cplusplus
#define VEC3(x, y, z) \
    Vector3 { x, y, z }
#else
#define VEC3(x, y, z) ((Vector3){x, y, z})
#endif

#define VEC3_ZERO VEC3(0.0f, 0.0f, 0.0f)
#define VEC3_ONE VEC3(1.0f, 1.0f, 1.0f)
#define VEC3_UP VEC3(0.0f, 1.0f, 0.0f)
#define VEC3_DOWN VEC3(0.0f, -1.0f, 0.0f)
#define VEC3_LEFT VEC3(-1.0f, 0.0f, 0.0f)
#define VEC3_RIGHT VEC3(1.0f, 0.0f, 0.0f)
#define VEC3_FORWARD VEC3(0.0f, 0.0f, 1.0f)
#define VEC3_BACK VEC3(0.0f, 0.0f, -1.0f)
    typedef float VXfloat;
    typedef unsigned int VXuint;

#define VX_WINDOW_CURSOR_DISABLED 0x00001000

    enum VX_MeshType
    {
        VX_MESH_CUBE = 0,
        VX_MESH_SPHERE = 1,
        VX_MESH_PLANE = 2,
        VX_MESH_TRIANGLE = 3,
        VX_MESH_QUAD = 4
    };

    enum VX_InitFlags
    {
        VX_LOAD_DEFAULT = 0
    };

    enum VX_ClearFlags
    {
        VX_CLEAR_COLOR = 1 << 0,
        VX_CLEAR_DEPTH = 1 << 1
    };

    typedef struct
    {
        float r, g, b, a;
    } vxColor;

    static const vxColor VX_BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
    static const vxColor VX_WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
    static const vxColor VX_RED = {1.0f, 0.0f, 0.0f, 1.0f};
    static const vxColor VX_GREEN = {0.0f, 1.0f, 0.0f, 1.0f};
    static const vxColor VX_BLUE = {0.0f, 0.0f, 1.0f, 1.0f};
    static const vxColor VX_YELLOW = {1.0f, 1.0f, 0.0f, 1.0f};
    static const vxColor VX_MAGENTA = {1.0f, 0.0f, 1.0f, 1.0f};
    static const vxColor VX_CYAN = {0.0f, 1.0f, 1.0f, 1.0f};
    static const vxColor VX_GRAY = {0.5f, 0.5f, 0.5f, 1.0f};
    static const vxColor VX_ORANGE = {1.0f, 0.5f, 0.0f, 1.0f};
    static const vxColor VX_PURPLE = {0.5f, 0.0f, 0.5f, 1.0f};
    static const vxColor VX_DARK_BLUE = {0.1f, 0.1f, 0.15f, 1.0f};

    enum VX_KeyCode
    {

        VX_KEY_A = 65,
        VX_KEY_B = 66,
        VX_KEY_C = 67,
        VX_KEY_D = 68,
        VX_KEY_E = 69,
        VX_KEY_F = 70,
        VX_KEY_G = 71,
        VX_KEY_H = 72,
        VX_KEY_I = 73,
        VX_KEY_J = 74,
        VX_KEY_K = 75,
        VX_KEY_L = 76,
        VX_KEY_M = 77,
        VX_KEY_N = 78,
        VX_KEY_O = 79,
        VX_KEY_P = 80,
        VX_KEY_Q = 81,
        VX_KEY_R = 82,
        VX_KEY_S = 83,
        VX_KEY_T = 84,
        VX_KEY_U = 85,
        VX_KEY_V = 86,
        VX_KEY_W = 87,
        VX_KEY_X = 88,
        VX_KEY_Y = 89,
        VX_KEY_Z = 90,

        VX_KEY_0 = 48,
        VX_KEY_1 = 49,
        VX_KEY_2 = 50,
        VX_KEY_3 = 51,
        VX_KEY_4 = 52,
        VX_KEY_5 = 53,
        VX_KEY_6 = 54,
        VX_KEY_7 = 55,
        VX_KEY_8 = 56,
        VX_KEY_9 = 57,
        VX_KEY_EQUAL = 336,

        VX_KEY_F1 = 290,
        VX_KEY_F2 = 291,
        VX_KEY_F3 = 292,
        VX_KEY_F4 = 293,
        VX_KEY_F5 = 294,
        VX_KEY_F6 = 295,
        VX_KEY_F7 = 296,
        VX_KEY_F8 = 297,
        VX_KEY_F9 = 298,
        VX_KEY_F10 = 299,
        VX_KEY_F11 = 300,
        VX_KEY_F12 = 301,

        VX_KEY_SPACE = 32,
        VX_KEY_ENTER = 257,
        VX_KEY_ESCAPE = 256,
        VX_KEY_BACKSPACE = 259,
        VX_KEY_TAB = 258,
        VX_KEY_SHIFT = 340,
        VX_KEY_LEFT_SHIFT = 340,
        VX_KEY_RIGHT_SHIFT = 344,
        VX_KEY_CONTROL = 341,
        VX_KEY_LEFT_CONTROL = 341,
        VX_KEY_RIGHT_CONTROL = 345,
        VX_KEY_ALT = 342,
        VX_KEY_LEFT_ALT = 342,
        VX_KEY_RIGHT_ALT = 346,
        VX_KEY_SUPER = 343,

        VX_KEY_UP = 265,
        VX_KEY_DOWN = 264,
        VX_KEY_LEFT = 263,
        VX_KEY_RIGHT = 262,

        VX_KEY_CAPS_LOCK = 280,
        VX_KEY_NUM_LOCK = 282,
        VX_KEY_SCROLL_LOCK = 281,
        VX_KEY_INSERT = 260,
        VX_KEY_DELETE = 261,
        VX_KEY_HOME = 268,
        VX_KEY_END = 269,
        VX_KEY_PAGE_UP = 266,
        VX_KEY_PAGE_DOWN = 267,

        VX_KEY_NUMPAD_0 = 320,
        VX_KEY_NUMPAD_1 = 321,
        VX_KEY_NUMPAD_2 = 322,
        VX_KEY_NUMPAD_3 = 323,
        VX_KEY_NUMPAD_4 = 324,
        VX_KEY_NUMPAD_5 = 325,
        VX_KEY_NUMPAD_6 = 326,
        VX_KEY_NUMPAD_7 = 327,
        VX_KEY_NUMPAD_8 = 328,
        VX_KEY_NUMPAD_9 = 329,
        VX_KEY_NUMPAD_ADD = 334,
        VX_KEY_NUMPAD_SUBTRACT = 333,
        VX_KEY_NUMPAD_MULTIPLY = 332,
        VX_KEY_NUMPAD_DIVIDE = 331,
        VX_KEY_NUMPAD_DECIMAL = 330,
        VX_KEY_NUMPAD_ENTER = 335,

        VX_KEY_ESC = VX_KEY_ESCAPE,
        VX_KEY_CTRL = VX_KEY_CONTROL,
        VX_KEY_LCTRL = VX_KEY_LEFT_CONTROL,
        VX_KEY_RCTRL = VX_KEY_RIGHT_CONTROL,
        VX_KEY_LSHIFT = VX_KEY_LEFT_SHIFT,
        VX_KEY_RSHIFT = VX_KEY_RIGHT_SHIFT,
        VX_KEY_LALT = VX_KEY_LEFT_ALT,
        VX_KEY_RALT = VX_KEY_RIGHT_ALT
    };

    enum VX_KeyState
    {
        VX_KEY_RELEASED = 0,
        VX_KEY_PRESSED = 1,
        VX_KEY_REPEAT = 2
    };

    VX_API int vxInit(int flags);
    VX_API void vxTerminate(void);

    VX_API vxWindow *vxCreateWindow(int width, int height, const char *title);
    VX_API int vxWindowShouldClose(vxWindow *window);
    VX_API void vxPollEvents(vxWindow *window);
    VX_API void vxSwapBuffers(vxWindow *window);
    VX_API void vxDestroyWindow(vxWindow *window);

    VX_API vxRenderContext *vxCreateRenderContext(vxWindow *window);
    VX_API void vxDestroyRenderContext(vxRenderContext *ctx);
    VX_API void vxClearColor(vxRenderContext *ctx, float r, float g, float b);
    VX_API void vxClear(vxRenderContext *ctx, int flags);

    VX_API int vxKeyPressed(vxWindow *window, int key);
    VX_API int vxKeyDown(vxWindow *window, int key);
    VX_API int vxKeyReleased(vxWindow *window, int key);
    VX_API void vxGetMousePosition(vxWindow *window, float *x, float *y);
    VX_API void vxGetMouseDelta(vxWindow *window, float *dx, float *dy);
    VX_API int vxMouseButtonPressed(vxWindow *window, int button);
    VX_API int vxMouseButtonDown(vxWindow *window, int button);
    VX_API void vxSetMouseLock(vxWindow *window, int lock);
    VX_API int vxGetMouseLock(vxWindow *window);

    VX_API vxMesh *vxNewMesh(int meshType);
    VX_API void vxDrawMesh(vxRenderContext *ctx, vxMesh *mesh, float r, float g, float b, float a, Vector3 position, float size, float rotation, float tilt);
    VX_API void vxDestroyMesh(vxMesh *mesh);

    VX_API void vxExitAndClean(vxWindow *window);

    VX_API void vxUpdateWindowSize(int width, int height);

    VX_API void vxDestroyAll(vxWindow *window, vxRenderContext *context, int count, vxMesh **objects);

    enum VX_WindowFlags
    {
        VX_WINDOW_RESIZABLE = 1 << 0,
        VX_WINDOW_DECORATED = 1 << 1,
        VX_WINDOW_FLOATING = 1 << 2,
        VX_WINDOW_MAXIMIZED = 1 << 3,
        VX_WINDOW_VISIBLE = 1 << 4,
        VX_WINDOW_AUTO_ICONIFY = 1 << 5,
        VX_WINDOW_TARGET_FPS = 1 << 6,
        VX_WINDOW_VSYNC = 1 << 7,
        VX_WINDOW_FULLSCREEN_BORDERLESS = 1 << 8,
        VX_WINDOW_CENTER_CURSOR = 1 << 9,
        VX_WINDOW_FOCUS_ON_SHOW = 1 << 10
    };

    VX_API void vxSetWindowIcon(vxWindow *window, const char *iconPath);
    VX_API void vxSetWindowFlags(vxWindow *window, int flags, int value);
    VX_API void vxGetWindowFlags(vxWindow *window, int *flags);
    VX_API void vxSetWindowTitle(vxWindow *window, const char *title);
    VX_API void vxSetWindowSize(vxWindow *window, int width, int height);
    VX_API void vxSetWindowPos(vxWindow *window, int x, int y);

    VX_API void vxSetTargetFPS(vxWindow *window, int fps);
    VX_API void vxLimitFPS(vxWindow *window);
    VX_API double vxGetCurrentFPS(vxWindow *window);

    typedef struct
    {
        float x, y, z;
        float r, g, b;
        float nx, ny, nz;
    } vxVertex;

#define VX_VERTEX(x, y, z, r, g, b) {(x), (y), (z), (r), (g), (b), 0.0f, 1.0f, 0.0f}
#define VX_VERTEX_NORMAL(x, y, z, r, g, b, nx, ny, nz) {(x), (y), (z), (r), (g), (b), (nx), (ny), (nz)}

    VX_API vxMesh *vxCreateMeshFromVertices(vxVertex *vertices, int vertexCount);
    VX_API vxMesh *vxCreateMeshFromArray(float *vertices, int vertexCount);

    VX_API void vxSetWindowFullscreen(vxWindow *window, int fullscreen, int borderless);
    VX_API void vxGetWindowSize(vxWindow *window, int *width, int *height);
    VX_API void vxGetScreenSize(int *width, int *height);

    enum VX_LightType
    {
        VX_LIGHT_DIRECTIONAL = 0,
        VX_LIGHT_POINT = 1,
        VX_LIGHT_SPOT = 2
    };

    VX_API void vxSetupLighting(int type, float r, float g, float b, float intensity, float x, float y, float z);
    VX_API void vxSetAmbientLight(float r, float g, float b);
#ifdef __cplusplus
}
#endif

#endif