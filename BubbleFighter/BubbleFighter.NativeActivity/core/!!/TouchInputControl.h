#pragma once

#include <memory>


namespace AApp
{

	class TouchInputControl
	{
	public:
		virtual bool isWithinBounds(Vec2f point) = 0; //point in screen coordinates
		virtual bool canMoveIn() = 0; //point in screen coordinates
		virtual bool canMoveOut() = 0; //point in screen coordinates

		virtual void onTouchDown(const Vec2f& pointer) {};
		virtual void onTouchMove(const Vec2f& pointer) {};
		virtual void onTouchMoveIn(const Vec2f& pointer) {};
		virtual void onTouchMoveOut(const Vec2f& pointer) {};
		virtual void onTouchUp(const Vec2f& pointer) {};

		virtual void onReset() {};
	};


	std::unique_ptr<TouchInputControl> UPTouchInputControl;
}
