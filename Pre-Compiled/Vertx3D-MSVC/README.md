# Vertx3D
[![Build Status](https://img.shields.io/badge/build-pre--alpha-red)](https://github.com/htcdevk0/Vertx3D)

Commit at: 01/18/2026
## Author

- [@htcdevk0](https://github.com/htcdevk0)

# What is Vertx3D?

Vertx3D is a 3D library/engine designed to provide an entry point into the world of OpenGL, offering ease of use without compromising the core principles of OpenGL.

# Dependencies

- OpenGL (GLFW)
- GLAD
- FT2build
- STB

# Platforms

- Window 10/11

# Compilers


- Mingw64 - MSYS2 (WINDOWS)
- MSVC v14.50 (WINDOWS)

# Supported at

- C++ 23
- C 23 (Limited)

# How to compile

- For MinGW-w64, it is necessary to either create a new CMake configuration or use and modify the one provided in the release.

- For MSVC, please refer to the tutorial in the DOCS.

## Example

```cpp
#include <vertx3d.h>

int main() {
	vxInit(VX_LOAD_DEFAULT);
	vxWindow* my_window = vxCreateWindow(800, 600, "Vertx3D - Example");
	vxRenderContext* my_ctx = vxCreateRenderContext(my_window);
	vxSetWindowFlags(my_window, VX_WINDOW_RESIZABLE, false);

	vxMesh* cube = vxNewMesh(VX_MESH_CUBE);

	while (!vxWindowShouldClose(my_window)) {
		vxPollEvents(my_window);
		vxClear(my_ctx, VX_CLEAR_COLOR | VX_CLEAR_DEPTH);
		vxClearColor(my_ctx, 0.2f, 0.2f, 0.2f);

		vxDrawMesh(
			my_ctx,
			cube,
			1.0f, 0.0f, 0.0f, 1.0f,
			Vector3{ 0.0f, 0.0f, 0.0f },
			2.0f,
			-25.0f,
			-15.0f
		);

		vxSwapBuffers(my_window);
	}

	vxDestroyWindow(my_window);
	vxDestroyRenderContext(my_ctx);
	vxDestroyMesh(cube);
	vxTerminate();
	return 0;
}
```
This example demonstrates how to create and render a simple 3D red cube using Vertx3D.

# How to Download

- Refer to the pre-compiled package available in the Releases section.

Have fun!