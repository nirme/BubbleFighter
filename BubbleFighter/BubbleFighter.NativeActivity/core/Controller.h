#pragma once

#include <memory>


namespace core
{
	class ControllerValue
	{

	public:
		virtual ~ControllerValue() {};
		virtual float get() const = 0;
		virtual void set(float _value) = 0;
	};

	class ControllerFunc
	{
	protected:

	public:
		virtual ~ControllerFunc() {};
		virtual float calculate(float _rawValue) = 0;
	};


	class Controller
	{
	protected:
		SharedControllerValuePtr sourceValue;
		SharedControllerValuePtr destinationValue;
		SharedControllerFuncPtr transformFunc;

		bool enabled;


	public:

		Controller(SharedControllerValuePtr _sourceValue, SharedControllerValuePtr _destinationValue, SharedControllerFuncPtr _transformFunc, bool _enabled = true);

		void setSource(SharedControllerValuePtr _sourceValue);
		void setDestination(SharedControllerValuePtr _destinationValue);
		void setTransformationFunction(SharedControllerFuncPtr _transformFunc);

		void setEnabled(bool _enabled = true);
		bool isEnabled();

		void update();



	};



	typedef std::shared_ptr<ControllerValue> SharedControllerValuePtr;
	typedef std::shared_ptr<ControllerFunc> SharedControllerFuncPtr;
	typedef std::shared_ptr<Controller> ControllerPtr;

}