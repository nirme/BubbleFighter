#pragma once


namespace core
{

	class InputHandler
	{
	public:
		virtual int onTouchEvent(AInputEvent* event) = 0;
		//virtual bool onKeyboardEvent(AInputEvent* event) = 0;
	};

}
