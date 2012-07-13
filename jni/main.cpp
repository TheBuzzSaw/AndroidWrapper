#include "Window.hpp"

void OnLoad(const XPG::Ammo& ammo)
{
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
}

void OnUnload(const XPG::Ammo& ammo)
{

}

void OnDraw(const XPG::Ammo& ammo)
{
	glClear(GL_COLOR_BUFFER_BIT);
	ammo.window->SwapBuffers();
}

void OnTouch(const XPG::Ammo& ammo)
{
	glClearColor(((float)ammo.x)/ ammo.window->Width(), (float)ammo.window->Angle(),
			((float)ammo.y)/ammo.window->Height(), 1);

	glClear(GL_COLOR_BUFFER_BIT);
	ammo.window->SwapBuffers();
}

void OnAccel(const XPG::Ammo& ammo)
{

}

void OnLowMemory(const XPG::Ammo& ammo)
{

}

void OnResize(const XPG::Ammo& ammo)
{

}

void android_main(struct android_app* state)
{
	XPG::Window window(state);
	window.OnLoad().Aim(OnLoad);
	window.OnUnload().Aim(OnUnload);
	window.OnDraw().Aim(OnDraw);
	window.OnTouch().Aim(OnTouch);
	window.OnAccel().Aim(OnAccel);
	window.OnResize().Aim(OnResize);
	window.Run();
}
