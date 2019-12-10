#pragma once

#include <vector>
#include <list>
#include <map>

#include "InputHandler.h"
#include "TouchInputControl.h"
#include "utils\vector.h"
#include "utils\HelperFunc.h"


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

	class InputManager : public InputHandler
	{
	private:

		Vec2i screenSize;
		Vec2f screenSizeCashedInverse;
		float whRatio;

		typedef std::list<TouchInputControl*> TouchInputList;
		typedef std::map<std::string, TouchInputList> TouchInputLayersMap;

		TouchInputLayersMap inputLayers;
		TouchInputLayersMap::iterator currentInputLayer;

		std::map<int32_t, std::pair<Vec2f, TouchInputControl*>> currentPointers;



	public:

		InputManager( int width, int height)
		{
			screenSize.x = width;
			screenSize.y = height;

			whRatio = (float)width / height;

			screenSizeCashedInverse.x = 1.0f / width * whRatio;
			screenSizeCashedInverse.y = 1.0f / height;
			// height is base size of 1 - pointers and touch inputs must use this scheme to match up correctly (no camera/projection scaling and keeping aspect ratio for rendered controls is required)
			// with that pointer wil be (x/width * whRatio, y/height)

			auto ret = inputLayers.insert(std::make_pair("", TouchInputList()));
			currentInputLayer = ret.first;
		};

		inline Vec2f getPointerPosition(AInputEvent* event, size_t pIndex)
		{
			return Vec2f(	AMotionEvent_getX(event, pIndex) * screenSizeCashedInverse.x, 
							AMotionEvent_getY(event, pIndex) * screenSizeCashedInverse.y);
		};


		bool createInputLayer(std::string _name)
		{
			return inputLayers.insert(std::make_pair(_name, TouchInputList())).second;
		};

		bool setInputLayer(std::string _name)
		{
			auto itb = inputLayers.find(_name);
			if (itb == inputLayers.end())
				return false;

			currentInputLayer = itb;

			for (auto it = currentPointers.begin(); it != currentPointers.end(); ++it)
				(*it).second.second->onReset();

			currentPointers.clear();

			return true;
		};


		void addInputControl(TouchInputControl* control)
		{
			TouchInputList& til = (*currentInputLayer).second;
			til.push_back(control);
		};


		void pointerDown(int32_t pid, const Vec2f& newPos)
		{
			//.find(pid);
			TouchInputList& controlList = (*currentInputLayer).second;
			for (auto it = controlList.begin(); it != controlList.end(); ++it)
			{
				if ((*it)->onTouchDown(newPos))
				{
					currentPointers.insert(std::make_pair(pid, std::make_pair(newPos, *it)));
					return;
				}
			}

			currentPointers.insert(std::make_pair(pid, std::make_pair(newPos, nullptr)));
		};

		void pointerMove(int32_t pid, const Vec2f& newPos)
		{
			auto pointer = currentPointers.find(pid);
			if ((*pointer).second.second != nullptr)
			{
				TouchInputControl* ctrl = (*pointer).second.second;

				if (!ctrl->canMoveOut() || ctrl->isWithinBounds(newPos))
				{
					ctrl->onTouchMove(newPos);
					return;
				}

				ctrl->onTouchMoveOut(newPos);
				(*pointer).second.second = nullptr;
			}

			TouchInputList& controlList = (*currentInputLayer).second;
			for (auto it = controlList.begin(); it != controlList.end(); ++it)
			{
				if ((*it)->canMoveIn() && (*it)->isWithinBounds(newPos))
				{
					(*pointer).second.second = (*it);
					(*it)->onTouchMoveIn(newPos);
					return;
				}
			}
		};

		void pointerUp(int32_t pid, const Vec2f& newPos)
		{
			auto pointer = currentPointers.find(pid);
			if ((*pointer).second.second != nullptr)
			{
				TouchInputControl* ctrl = (*pointer).second.second;

				if (!ctrl->canMoveOut() || ctrl->isWithinBounds(newPos))
					ctrl->onTouchUp(newPos);
				else
					ctrl->onTouchMoveOut(newPos);
			}

			currentPointers.erase(pid);
		};



	public:
		virtual int onTouchEvent(AInputEvent* event)
		{
			//pointersPosition

			int32_t action = AMotionEvent_getAction(event);
			if (action < 0) return action;

			int flags = action & AMOTION_EVENT_ACTION_MASK;

			size_t index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

			int32_t pid = AMotionEvent_getPointerId(event, index);
			if (pid < 0) return pid;

			Vec2f v2 = getPointerPosition(event, index);

			switch (flags)
			{
			case AMOTION_EVENT_ACTION_UP:
			case AMOTION_EVENT_ACTION_POINTER_UP:
				pointerUp(pid, v2);
				return;

			case AMOTION_EVENT_ACTION_MOVE:
				pointerMove(pid, v2);
				return;

			case AMOTION_EVENT_ACTION_DOWN:
			case AMOTION_EVENT_ACTION_POINTER_DOWN:
				pointerDown(pid, v2);
				return;
			}

			return 0;
		};

		//virtual int onKeyboardEvent(AInputEvent* pEvent) = 0;
	};

}
