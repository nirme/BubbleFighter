#pragma once


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



namespace AApp
{
	class ActivityHandler
	{
	public:

		virtual void onActivate() = 0;
		virtual void onDeactivate() = 0;

		virtual void onStep() = 0;


		virtual void onInitWindow() {};
		virtual void onTermWindow() {};
		virtual void onGainedFocus() {};
		virtual void onLostFocus() {};
		virtual void onConfigChanged() {};

		virtual void onLowMemory() {};
		virtual void onStart() {};
		virtual void onResume(void* pData, size_t pSize) {};

		virtual void onSaveState(void** pData, size_t* pSize) {};
		virtual void onPause() {};
		virtual void onStop() {};
		virtual void onDestroy() {};

	};


	class InputHandler
	{
	public:
		virtual bool onTouchEvent(AInputEvent* pEvent) = 0;
		//virtual bool onKeyboardEvent(AInputEvent* pEvent) = 0;
	};

}
