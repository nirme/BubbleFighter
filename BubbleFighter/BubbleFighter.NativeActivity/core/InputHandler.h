#pragma once


namespace AApp
{

	class InputHandler
	{
	public:
		virtual bool onTouchEvent(AInputEvent* event) = 0;
		//virtual bool onKeyboardEvent(AInputEvent* event) = 0;
	};

}
