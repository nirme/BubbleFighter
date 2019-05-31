#pragma once

#include <vector>
#include <list>
#include <map>

#include "InputHandler.h"
#include "TouchInputControl.h"
#include "vec.h"
#include "HelperFunc.h"


namespace AApp
{

	class InputManager : public InputHandler
	{
	private:

		Vec2i screenSize;
		Mx3d toScreenCoordMatrix;

		std::map<int32_t, Vec2i> currentPointers;


		typedef std::list<TouchInputControl*> TouchInputList;
		typedef std::map<std::string, TouchInputList> TouchInputMap;
		typedef std::map<int32_t, TouchInputControl*> PointedTouchInputMap;

		TouchInputMap inputLayers;
		TouchInputList* currentInputLayer;

		PointedTouchInputMap inputsWithPointerDown;



		InputManager( int width, int height)
		{
			auto ret = inputLayers.insert(std::make_pair("", TouchInputList()));
			currentInputLayer = &((*ret.first).second);

			screenSize.x = width;
			screenSize.y = height;
			create2DProjectionMatrix(toScreenCoordMatrix, width, height, 1.f);
		};

		inline Vec2f translatePointerToScreenCoord(Vec2i point)
		{
			Vec2f out((float)point.x / screenSize.x, (float)point.y / screenSize.y);
			return toScreenCoordMatrix.mul(out);
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

			currentInputLayer = &(*itb).second;

			for (auto it = inputsWithPointerDown.begin(); it != inputsWithPointerDown.end(); ++it)
				(*it).second->onReset();

			inputsWithPointerDown.clear();
			currentPointers.clear();

			return true;
		};

		void addInputControl(TouchInputControl* control)
		{
			currentInputLayer->push_back(control);
		};




		void pointerDown(int32_t pid, const Vec2i& newPos)
		{};

		void pointerMove(int32_t pid, const Vec2i& oldPos)
		{};

		void pointerUp(int32_t pid, const Vec2i& lastPos)
		{
			auto it = inputsWithPointerDown.find(pid);
			if (it != inputsWithPointerDown.end())
			{
				if ((*it).second->isWithinBounds())
					(*it).second->isWithinBounds
			}

			currentInputLayer->


			//std::map<int32_t, Vec2i>
			currentPointers.find(pid)

		
		};



	public:
		virtual bool onTouchEvent(AInputEvent* event)
		{
			//pointersPosition

			int32_t action = AMotionEvent_getAction(event);
			int flags = action & AMOTION_EVENT_ACTION_MASK;

			size_t index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

			int32_t pid = AMotionEvent_getPointerId(event, index);


			switch (flags)
			{
			case AMOTION_EVENT_ACTION_UP:
			case AMOTION_EVENT_ACTION_POINTER_UP:
			{}
			case AMOTION_EVENT_ACTION_MOVE:
			{}
			case AMOTION_EVENT_ACTION_DOWN:
			case AMOTION_EVENT_ACTION_POINTER_DOWN:
			{}
			}


				size_t AMotionEvent_getPointerCount(
					const AInputEvent *motion_event
				)

		};
		//virtual bool onKeyboardEvent(AInputEvent* pEvent) = 0;
	};

}
