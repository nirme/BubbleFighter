#pragma once

#include "ActivityHandler.h"
#include "InputHandler.h"


//activityHandler->onStep();

namespace AApp
{
	class EventLoop
	{
	protected:

		bool active;

		android_app* androidApp;

		ActivityHandler* activityHandler;
		InputHandler* inputHandler;


	private:

		void run()
		{

			int ret;
			int events;
			android_poll_source* source;

			while (true) {
				// Read all pending events.
				while ((ret = ALooper_pollAll(active ? 0 : -1, NULL, &events, (void**) &source)) >= 0)
				{
					if (source != NULL)
					{
						source->process(androidApp, source);
					}

					// Check if we are exiting.
					if (androidApp->destroyRequested != 0)
					{
						engine->termDisplay();
						return;
					}
				}

				if (engine->haveContext())
				{
					//Vec3F v3f(1.f, 0.f,1.f);
					//engine->clearFrame(&(engine->v3));

					//engine->getVideoDriver()->render(engine->bg);
					engine->render();

					//TODO
					//render all here
					//fps counter
					// decouple render and draw and throttle rendering to max refresh rate

					engine->showFrame();
				}
			}


		};


		void activate()
		{
			activityHandler->onActivate();

			active = true;
		};

		void deactivate()
		{
			activityHandler->onDeactivate();


			active = false;
		};


		static void onApplicationCommand(android_app* app, int32_t cmd)
		{
			EventLoop& eventLoop = *(EventLoop*)app->userData;
			eventLoop.processAppEvent(cmd);
		};



		static int32_t onInputEvent(android_app* app, AInputEvent* event)
		{
			EventLoop& eventLoop = *(EventLoop*)app->userData;
			return eventLoop.processInputEvent(event);
		};



		void processAppEvent(int32_t cmd)
		{
			switch (cmd)
			{
				case APP_CMD_INIT_WINDOW:
					activityHandler->onInitWindow();
					return;

				case APP_CMD_TERM_WINDOW:
					activityHandler->onTermWindow();
					deactivate();
					return;

				case APP_CMD_GAINED_FOCUS:
					activate();
					activityHandler->onGainedFocus();
					return;

				case APP_CMD_LOST_FOCUS:
					activityHandler->onLostFocus();
					deactivate();
					return;

				case APP_CMD_CONFIG_CHANGED:
					activityHandler->onConfigChanged();
					return;

				case APP_CMD_LOW_MEMORY:
					activityHandler->onLowMemory();
					return;

				case APP_CMD_START:
					activityHandler->onStart();
					return;

				case APP_CMD_RESUME:
					activityHandler->onResume(androidApp->savedState, androidApp->savedStateSize);
					return;

				case APP_CMD_SAVE_STATE:
					activityHandler->onSaveState(&(androidApp->savedState), &(androidApp->savedStateSize));
					return;

				case APP_CMD_PAUSE:
					activityHandler->onPause();
					deactivate();
					return;

				case APP_CMD_STOP:
					activityHandler->onStop();
					return;

				case APP_CMD_DESTROY:
					activityHandler->onDestroy();
					deactivate();
					return;
			}
		};


		int32_t processInputEvent(AInputEvent* event)
		{
			switch (AInputEvent_getType(event))
			{
			case AINPUT_EVENT_TYPE_MOTION:

				switch (AInputEvent_getSource(event))
				{
				case AINPUT_SOURCE_TOUCHSCREEN:
					inputHandler->onTouchEvent(event);
					break;

				//case AINPUT_SOURCE_DPAD:
				//case AINPUT_SOURCE_GAMEPAD:
				//case AINPUT_SOURCE_JOYSTICK:
				//case AINPUT_SOURCE_MOUSE:
				//case AINPUT_SOURCE_KEYBOARD:
				}

				break;

			//case AINPUT_EVENT_TYPE_KEY:
			}

			return 0;
		};


	};

}
