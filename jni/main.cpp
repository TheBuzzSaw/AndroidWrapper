#include "Window.hpp"

void android_main(struct android_app* state)
{
	XPG::Window window(state);
	window.Run();
}
