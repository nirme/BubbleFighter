#pragma once

#include <algorithm>
#include <vector>
#include <list>
#include <unordered_map>

#include "InputHandler.h"
#include "Vector2.h"
#include "TouchControl.h"


namespace core
{

	class InputManager : public InputHandler
	{
	private:
		struct ScreenPosition
		{
			unsigned short x;
			unsigned short y;
		};

		ScreenPosition screenSize;
		Vector2 inverseScreenSize;

		typedef std::vector<TouchControl*> TouchControlList;
		typedef std::unordered_map<std::string, TouchControlList> TouchControlSets;
		typedef TouchControlSets::iterator TouchControlSetsIterator;

		TouchControlSets controlSets;
		std::string currentSetName;

		struct PointerData
		{
			int32_t pointerId;
			Vector2 position;
			Vector2 screenSpace;
			TouchControl* affectedControl;
		};

		typedef std::unordered_map<int32_t, PointerData> ActivePointerList;
		ActivePointerList activePointers;


		void removeFromActivePointers(TouchControl *_control)
		{
			std::vector<int32_t> idToRemove;
			for (auto it = activePointers.begin(), itEnd = activePointers.end(); it != itEnd; ++it)
				if ((*it).second.affectedControl == _control)
					idToRemove.push_back((*it).first);

			while (idToRemove.size())
			{
				activePointers.erase(idToRemove.back());
				idToRemove.pop_back();
			}

		};


	public:

		InputManager() :
			screenSize{1, 1},
			inverseScreenSize{ 1.0f, 1.0f },
			currentSetName("")
		{};


		void setSize(unsigned short _width, unsigned short _height)
		{
			screenSize = { _width, _height };
			inverseScreenSize = Vector2(1.0f / screenSize.x, 1.0f / screenSize.y);
		};


		void addControl(const std::string &_setName, TouchControl *_control)
		{
			assert(_control || "cannot add nullptr as touch control");

			auto res = controlSets.emplace(_setName, TouchControlList());
			TouchControlList &list = (*res.first).second;

			auto it = std::find(list.begin(), list.end(), _control);
			if (it == list.end())
				list.push_back(_control);
		};


		void removeControl(const std::string &_setName, TouchControl *_control)
		{
			assert(_control || "cannot remove nullptr as touch control");

			auto it = controlSets.find(_setName);
			if (it != controlSets.end())
			{
				auto it2 = std::find((*it).second.begin(), (*it).second.end(), _control);
				if (it2 != (*it).second.end())
				{
					removeFromActivePointers(_control);

					std::swap(*it2, (*it).second.back());
					(*it).second.pop_back();
				}
			}
		};

		void removeControlSet(const std::string &_setName)
		{
			auto setIt = controlSets.find(_setName);

			if (setIt != controlSets.end())
			{
				if (currentSetName == _setName)
					currentSetName.clear();

				TouchControlList &list = (*setIt).second;
				for (auto it = list.begin(), itEnd = list.end(); it != itEnd; ++it)
					removeFromActivePointers(*it);
			}

			controlSets.erase(_setName);
		};

		void removeAllControls()
		{
			controlSets.clear();
			activePointers.clear();
		};


		void activateControlSet(const std::string &_setName)
		{
			currentSetName = _setName;
		};



		TouchControl *getAffectedControl(int32_t _pointerId, const Vector2 &_pointerPosition)
		{
			if (currentSetName.empty())
				return nullptr;

			auto setIt = controlSets.find(currentSetName);
			if (setIt != controlSets.end())
				return nullptr;

			TouchControlList &controls = (*setIt).second;

			for (auto it = controls.begin(), itEnd = controls.end(); it != itEnd; ++it)
				if ((*it)->containsPointer(_pointerId, _pointerPosition))
					return (*it);

			return nullptr;
		};


		int onTouchEvent(AInputEvent* _event)
		{
			int32_t action = AMotionEvent_getAction(_event);
			if (action < 0) return action;

			int flags = action & AMOTION_EVENT_ACTION_MASK;
			size_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

			int32_t pointerId = AMotionEvent_getPointerId(_event, pointerIndex);
			if (pointerId < 0) return pointerId;

			Vector2 pointerPosition = { 
				AMotionEvent_getX(_event, pointerIndex),
				AMotionEvent_getY(_event, pointerIndex)
			};

			Vector2 pointerScreenPosition = {
				pointerPosition.x * inverseScreenSize.x,
				pointerPosition.y * inverseScreenSize.y
			};


			TouchControl *affectedPointer = getAffectedControl(pointerId, pointerScreenPosition);
			if (!affectedPointer)
			{
				// remove active pointer if moved out
				auto it = activePointers.find(pointerId);
				if (it != activePointers.end())
				{
					(*it).second.affectedControl->onPointerMoveOut((*it).second.pointerId, pointerScreenPosition);
					activePointers.erase(it);
				}

				return 0;
			}


			switch (flags)
			{
				case AMOTION_EVENT_ACTION_UP:
				case AMOTION_EVENT_ACTION_POINTER_UP:
				{
					auto it = activePointers.find(pointerId);
					PointerData &pDataRes = (*it).second;

					pDataRes.affectedControl->onPointerUp(pDataRes.pointerId, pDataRes.screenSpace);
					activePointers.erase(it);

					break;
				}

				case AMOTION_EVENT_ACTION_MOVE:
				{
					auto it = activePointers.find(pointerId);
					if (it != activePointers.end())
					{
						PointerData &pDataRes = (*it).second;
						pDataRes.position = pointerPosition;
						pDataRes.screenSpace = pointerScreenPosition;

						if (pDataRes.affectedControl == affectedPointer)
						{
							pDataRes.affectedControl->onPointerMove(pDataRes.pointerId, pDataRes.screenSpace);
						}
						else
						{
							pDataRes.affectedControl->onPointerMoveOut(pDataRes.pointerId, pDataRes.screenSpace);

							pDataRes.affectedControl = affectedPointer;
							pDataRes.affectedControl->onPointerMoveIn(pDataRes.pointerId, pDataRes.screenSpace);
						}
					}
					else
					{
						PointerData pData = { pointerId, pointerPosition, pointerScreenPosition, affectedPointer };
						activePointers.emplace(pointerId, pData);
						pData.affectedControl->onPointerMoveIn(pData.pointerId, pData.screenSpace);
					}

					break;
				}

				case AMOTION_EVENT_ACTION_DOWN:
				case AMOTION_EVENT_ACTION_POINTER_DOWN:
				{
					PointerData pData = { pointerId, pointerPosition, pointerScreenPosition, affectedPointer };

					auto res = activePointers.emplace(pointerId, pData);
					PointerData &pDataRes = (*res.first).second;

					pDataRes.affectedControl->onPointerDown(pDataRes.pointerId, pDataRes.screenSpace);

					break;
				}
			}


			return 0;
		};




	};

}
