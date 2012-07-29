#include "Window.hpp"

namespace XPG
{
	struct StupidState
	{
		float angle;
		int32_t x;
		int32_t y;
	};

	Window::Window(struct android_app* app)
	{
		_app = app;
		app_dummy();

		_animating = 0;
		_display = EGL_NO_DISPLAY;
		_context = EGL_NO_CONTEXT;
		_surface = EGL_NO_SURFACE;

		_app->userData = this;
		_app->onAppCmd = GetCommand;
		_app->onInputEvent = GetInput;

		_sensorManager = ASensorManager_getInstance();
		_accelerometerSensor = ASensorManager_getDefaultSensor
			(_sensorManager, ASENSOR_TYPE_ACCELEROMETER);

		_sensorEventQueue = ASensorManager_createEventQueue(_sensorManager,
			_app->looper, LOOPER_ID_USER, NULL, NULL);

		if (_app->savedState != NULL)
		{
			struct StupidState* ss = (struct StupidState*)_app->savedState;
			_angle = ss->angle;
			_x = ss->x;
			_y = ss->y;
		}
	}

	Window::~Window()
	{
		Close();
	}

	void Window::Open()
	{
		const EGLint attribs[] = {
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_BLUE_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_RED_SIZE, 8,
				EGL_NONE
		};

		EGLint w, h, dummy, format;
		EGLint numConfigs;
		EGLConfig config;
		EGLSurface surface;
		EGLContext context;

		EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(display, 0, 0);

		/* Here, the application chooses the configuration it desires. In this
		 * sample, we have a very simplified selection process, where we pick
		 * the first EGLConfig that matches our criteria */
		eglChooseConfig(display, attribs, &config, 1, &numConfigs);

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		 * As soon as we picked a EGLConfig, we can safely reconfigure the
		 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
		eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

		ANativeWindow_setBuffersGeometry(_app->window, 0, 0, format);

		surface = eglCreateWindowSurface(display, config, _app->window, NULL);

		const EGLint contextAttributes[] =
			{
				EGL_CONTEXT_CLIENT_VERSION, 2,
				EGL_NONE
			};

		context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributes);

		if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
			LOGW("Unable to eglMakeCurrent");
			return;
		}

		eglQuerySurface(display, surface, EGL_WIDTH, &w);
		eglQuerySurface(display, surface, EGL_HEIGHT, &h);

		_display = display;
		_context = context;
		_surface = surface;
		_width = w;
		_height = h;
		_angle = 0;
		_animating = 1;

		// Initialize GL state.
		glDisable(GL_DEPTH_TEST);

		LOGI("OpenGL Version: %s", glGetString(GL_VERSION));

		Ammo ammo;
		ammo.window = this;
		ammo.x = _width;
		ammo.y = _height;
		_onLoad.Fire(ammo);
	}

	void Window::Close()
	{
		_animating = 0;

		Ammo ammo;
		ammo.window = this;
		_onUnload.Fire(ammo);

		if (_display != EGL_NO_DISPLAY)
		{
			eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
				EGL_NO_CONTEXT);

			if (_context != EGL_NO_CONTEXT)
			{
				eglDestroyContext(_display, _context);
			}

			if (_surface != EGL_NO_SURFACE)
			{
				eglDestroySurface(_display, _surface);
			}

			eglTerminate(_display);
		}

		_animating = 0;
		_display = EGL_NO_DISPLAY;
		_context = EGL_NO_CONTEXT;
		_surface = EGL_NO_SURFACE;
	}

	void Window::Run()
	{
		while (1)
		{
			int ident;
			int events;

			struct android_poll_source* source;

			while ((ident=ALooper_pollAll(_animating ? 0 : -1, NULL, &events,
				(void**)&source)) >= 0)
			{
				if (source != NULL)
				{
					source->process(_app, source);
				}

				// If a sensor has data, process it now.
				if (ident == LOOPER_ID_USER)
				{
					if (_accelerometerSensor != NULL)
					{
						ASensorEvent event;
						while (ASensorEventQueue_getEvents(_sensorEventQueue,
							&event, 1) > 0)
						{
							//LOGI("accelerometer: x=%f y=%f z=%f",
							//		event.acceleration.x,
							//		event.acceleration.y,
							//		event.acceleration.z);
						}
					}
				}

				// Check if we are exiting.
				if (_app->destroyRequested != 0)
				{
					Close();
					return;
				}
			}

			if (_animating)
			{
				// Done with events; draw next animation frame.
				_angle += .01f;
				if (_angle > 1)
				{
					_angle = 0;
				}

				// Drawing is throttled to the screen update rate, so there
				// is no need to do timing here.
				Ammo ammo;
				ammo.window = this;
				_onLoop.Fire(ammo);
			}
		}
	}

	void Window::SwapBuffers()
	{
		eglSwapBuffers(_display, _surface);
	}

	void Window::Draw()
	{
		if (_display != NULL)
		{
			Ammo ammo;
			ammo.window = this;
			_onDraw.Fire(ammo);
		}
	}

	void Window::GetCommand(struct android_app* app, int32_t command)
	{
	    Window* window = (Window*)app->userData;
	    window->OnCommand(app, command);
	}

	void Window::OnCommand(struct android_app* app, int32_t command)
	{
		LOGI("command %d", command);

	    switch (command)
	    {
			case APP_CMD_SAVE_STATE:
			{
				struct StupidState* ss =
					(struct StupidState*)malloc(sizeof(struct StupidState));
				ss->angle = _angle;
				ss->x = _x;
				ss->y = _y;

				_app->savedState = ss;
				_app->savedStateSize = sizeof(struct StupidState);
				break;
			}

	        case APP_CMD_INIT_WINDOW:
	            // The window is being shown, get it ready.
	            if (_app->window != NULL)
	            {
	                Open();
	                Draw();
	            }
	            break;

	        case APP_CMD_TERM_WINDOW:
	            // The window is being hidden or closed, clean it up.
	            Close();
	            break;

	        case APP_CMD_GAINED_FOCUS:
	            // When our app gains focus, we start monitoring the accelerometer.
	            if (_accelerometerSensor != NULL) {
	                ASensorEventQueue_enableSensor(_sensorEventQueue,
						_accelerometerSensor);
	                // We'd like to get 60 events per second (in us).
	                ASensorEventQueue_setEventRate(_sensorEventQueue,
	                        _accelerometerSensor, (1000L/60)*1000);
	            }

	            _animating = 1;
	            break;

	        case APP_CMD_LOST_FOCUS:
	            // When our app loses focus, we stop monitoring the accelerometer.
	            // This is to avoid consuming battery while not being used.
	            if (_accelerometerSensor != NULL)
	            {
	                ASensorEventQueue_disableSensor(_sensorEventQueue,
						_accelerometerSensor);
	            }
	            // Also stop animating.
	            _animating = 0;
	            break;
	    }
	}

	int32_t Window::OnInput(struct android_app* app, AInputEvent* event)
	{
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
		{
			_animating = 1;

			Ammo ammo;
			ammo.window = this;
			ammo.x = AMotionEvent_getX(event, 0);
			ammo.y = AMotionEvent_getY(event, 0);
			_onTouch.Fire(ammo);

			return 1;
		}

		return 0;
	}

	int32_t Window::GetInput(struct android_app* app, AInputEvent* event)
	{
		Window* window = (Window*)app->userData;
		return window->OnInput(app, event);
	}
}
