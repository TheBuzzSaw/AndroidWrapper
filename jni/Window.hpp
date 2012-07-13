#ifndef XpgWindowHpp
#define XpgWindowHpp

#include "Event.hpp"

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

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

			inline Event& OnDraw() { return _onDraw; }
			inline Event& OnLoad() { return _onLoad; }
			inline Event& OnUnload() { return _onUnload; }
			inline Event& OnTouch() { return _onTouch; }
			inline Event& OnAccel() { return _onAccel; }
			inline Event& OnLowMemory() { return _onLowMemory; }
			inline Event& OnResize() { return _onResize; }

			inline int32_t Width() const { return _width; }
			inline int32_t Height() const { return _height; }
			inline float Angle() const { return _angle; }

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

			Event _onDraw;
			Event _onLoad;
			Event _onUnload;
			Event _onTouch;
			Event _onAccel;
			Event _onLowMemory;
			Event _onResize;

			void Draw();
			void OnCommand(struct android_app* app,int32_t command);
			int32_t OnInput(struct android_app* app, AInputEvent* event);

			static void GetCommand(struct android_app* app,int32_t command);
			static int32_t GetInput(struct android_app* app, AInputEvent* event);
	};
}
#endif
