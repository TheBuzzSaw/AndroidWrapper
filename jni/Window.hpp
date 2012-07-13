#ifndef XpgWindowHpp
#define XpgWindowHpp

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

namespace XPG
{
	class Window
	{
		public:
			Window(struct android_app* app);
			~Window();

			void Open();
			void Close();

			void Run();

			void SwapBuffers();

		private:
			struct android_app* _app;
			ASensorManager* _sensorManager;
			const ASensor* _accelerometerSensor;
			ASensorEventQueue* _sensorEventQueue;

			int _animating;
			EGLDisplay _display;
			EGLSurface _surface;
			EGLContext _context;
			int32_t _width;
			int32_t _height;
			float _angle;
			int32_t _x;
			int32_t _y;

			void OnCommand(struct android_app* app,int32_t command);
			int32_t OnInput(struct android_app* app, AInputEvent* event);
			void OnDraw();

			static void GetCommand(struct android_app* app,int32_t command);
			static int32_t GetInput(struct android_app* app, AInputEvent* event);
	};
}
#endif
