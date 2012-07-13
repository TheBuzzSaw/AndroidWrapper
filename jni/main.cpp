#include "Window.hpp"

void OnDraw(const XPG::Ammo& ammo)
{
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ammo.window->SwapBuffers();
}

void android_main(struct android_app* state)
{
	XPG::Window window(state);
	window.OnDraw().Aim(OnDraw);
	window.Run();
}
