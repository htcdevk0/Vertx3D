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