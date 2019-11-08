#pragma once


#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <vector>

#include "Exceptions.h"

//	APP_CMD_INPUT_CHANGED	//	used by app glue internaly
//	APP_CMD_WINDOW_RESIZED	//	unused, app fullscreen only
//	APP_CMD_WINDOW_REDRAW_NEEDED	//	unused/don't care
//	APP_CMD_CONTENT_RECT_CHANGED	//	consider using, soft buttons could interfere

/*
APP_CMD_INIT_WINDOW	//	new ANativeWindow is ready for use. Upon receiving this command, android_app->window will contain the new window surface
APP_CMD_TERM_WINDOW	//	existing ANativeWindow needs to be terminated. Upon receiving this command, android_app->window still contains the existing window; after calling android_app_exec_cmd it will be set to NULL
APP_CMD_GAINED_FOCUS	//	app's activity window has gained input focus
APP_CMD_LOST_FOCUS	//	app's activity window has lost input focus
APP_CMD_CONFIG_CHANGED	//	Command from main thread: the current device configuration has changed
APP_CMD_LOW_MEMORY	//	the system is running low on memory, try to reduce your memory use
APP_CMD_START	//	app's activity has been started
APP_CMD_RESUME	//	app's activity has been resumed
APP_CMD_SAVE_STATE	//	app should generate a new saved state for itself, to restore from later if needed. If you have saved state, allocate it with malloc and place it in android_app.savedState with the size in android_app.savedStateSize. The will be freed for you later
APP_CMD_PAUSE	//	the app's activity has been paused
APP_CMD_STOP	//	app's activity has been stopped
APP_CMD_DESTROY	//	app's activity is being destroyed and waiting for the app thread to clean up and exit before proceeding
*/



namespace core
{

	class Engine
	{
	private:
		android_app* androidApp;

		bool initialized;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;

		float angle;





	public:

		Engine(android_app* _androidApp) :
			androidApp(_androidApp), angle(0.0f), initialized(false)
		{};


		void onActivate() {};
		void onDeactivate() {};

		void onStep()
		{
		    if (!initialized)
		        return;

			float red = angle <= 1.0f ? angle : (angle <= 2.0f ? (2.0f - angle) : 0);
			float green = angle <= 1.0f ? 0.0f : (angle <= 2.0f ? (angle - 1.0f) : (3.0f - angle));
			float blue = angle <= 1.0f ? (1.0f - angle) : (angle <= 2.0f ? 0 : (angle - 1.0f));

			glClearColor(red, green, blue, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

            if (eglSwapBuffers(display, surface) != EGL_TRUE)
			    return;

			angle += 0.01f;
			if (angle > 3.0f)
				angle = 0.0f;





		};



        EGLConfig getMatchigConfig(const EGLint *attributes)
        {
            EGLint numConfigs;

            EGL_ERROR_CHECK(eglChooseConfig(display, attributes, nullptr, 0, &numConfigs));

            std::vector<EGLConfig> configs(numConfigs);
            EGL_ERROR_CHECK(eglChooseConfig(display, attributes, configs.data(), numConfigs, &numConfigs));

            for (long i = 0; i < numConfigs; ++i)
            {
                const EGLint *attributePair = attributes;
                EGLint attributeValue;

                bool configMatching = true;
                while (attributePair[0] != EGL_NONE)
                {
                    EGL_ERROR_CHECK(eglGetConfigAttrib(display, configs[i], attributePair[0], &attributeValue));

                    if (attributePair[1] != attributeValue)
                    {
                        configMatching = false;
                        break;
                    }

                    attributePair += 2;
                }

                if (configMatching) return configs[i];
            }

            throw std::runtime_error("No frame buffer configuration matching selected attributes");
        };



		void onInitWindow()
		{
            const EGLint attribs[] = {
                    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                    EGL_BLUE_SIZE, 8,
                    EGL_GREEN_SIZE, 8,
                    EGL_RED_SIZE, 8,
                    EGL_NONE
            };

			display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			eglInitialize(display, 0, 0);

			/* Here, the application chooses the configuration it desires. In this
			* sample, we have a very simplified selection process, where we pick
			* the first EGLConfig that matches our criteria */

            EGLint w, h, format;

            EGLConfig config = getMatchigConfig(attribs);


			/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
			* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
			* As soon as we picked a EGLConfig, we can safely reconfigure the
			* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
			eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

			ANativeWindow_setBuffersGeometry(androidApp->window, 0, 0, format);

			surface = eglCreateWindowSurface(display, config, androidApp->window, NULL);
			context = eglCreateContext(display, config, NULL, NULL);

			if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
				//LOGW("Unable to eglMakeCurrent");
				return;
			}

			eglQuerySurface(display, surface, EGL_WIDTH, &w);
			eglQuerySurface(display, surface, EGL_HEIGHT, &h);

			width = w;
			height = h;
			angle = 0.0f;

			// Initialize GL state.
			//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
			glEnable(GL_CULL_FACE);
			//glShadeModel(GL_SMOOTH);
			glDisable(GL_DEPTH_TEST);

            initialized = true;
		};


		void onTermWindow()
		{
		
			if (display != EGL_NO_DISPLAY) {
				eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
				if (context != EGL_NO_CONTEXT) {
					eglDestroyContext(display, context);
				}
				if (surface != EGL_NO_SURFACE) {
					eglDestroySurface(display, surface);
				}
				eglTerminate(display);
			}
			display = EGL_NO_DISPLAY;
			context = EGL_NO_CONTEXT;
			surface = EGL_NO_SURFACE;

            initialized = false;
		};

		void onGainedFocus() {};

		void onLostFocus() {};
		void onConfigChanged() {};

		void onLowMemory() {};
		void onStart() {};
		void onResume(void* pData, size_t pSize) {};

		void onSaveState(void** pData, size_t* pSize) {};
		void onPause() {};
		void onStop() {};
		void onDestroy() {};

	};

}
