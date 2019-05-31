#pragma once

#include <memory>


namespace AApp
{

	class TouchInputControl
	{
	public:
		virtual bool isWithinBounds(Vec2f point) = 0; //point in screen coordinates

		virtual bool onTouchDown() {};
		virtual bool onTouchMove() {};
		virtual bool onTouchUp() {};

		virtual bool onReset() {};
	};


	std::unique_ptr<TouchInputControl> UPTouchInputControl;
}
