#pragma once

#include "Vector2.h"

namespace core
{

	class TouchControl
	{
	public:

		virtual bool containsPointer(int32_t _pointerId, const Vector2 &_pointerPosition) const = 0;

		virtual void onPointerDown(int32_t _pointerId, const Vector2 &_pointerPosition) = 0;
		virtual void onPointerUp(int32_t _pointerId, const Vector2 &_pointerPosition) = 0;
		virtual void onPointerMove(int32_t _pointerId, const Vector2 &_pointerPosition) = 0;

		virtual void onPointerMoveIn(int32_t _pointerId, const Vector2 &_pointerPosition) = 0;
		virtual void onPointerMoveOut(int32_t _pointerId, const Vector2 &_pointerPosition) = 0;
	};

}
