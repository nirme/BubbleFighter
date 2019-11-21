#pragma once


#include "Matrix3.h"
#include "_2d/Renderable.h"
#include "_2d/Camera.h"

namespace core
{
	class Camera;

	class ShadingParamsPassthru
	{
		const _2d::Renderable *renderable;
		const _2d::Camera *camera;

		Vector3 ambientLight;
		float timeElapsed;


	public:

		inline const Matrix3 &get2dWorldMatrix() const
		{
			return renderable ? renderable->getTransform() : Matrix3::IDENTITY;
		};

		inline const Matrix3 &get2dViewMatrix() const
		{
			return camera ? camera->getViewMatrix() : Matrix3::IDENTITY;
		};

		Matrix3 get2dWorldViewMatrix() const
		{
			return get2dViewMatrix() * get2dWorldMatrix();
		};

		inline const Vector3 &getAmbientLight()
		{
			return ambientLight;
		};

		inline float getTimeElapsed()
		{
			return timeElapsed;
		};


		inline void setCurrentRenderable(const _2d::Renderable *_renderable)
		{
			renderable = _renderable;
		};

		inline void setCamera(const _2d::Camera *_camera)
		{
			camera = _camera;
		};

		inline void setAmbientLightColor(Vector3 _ambientLight)
		{
			ambientLight = _ambientLight;
		};

		inline void setTimeElapsed(float _timeElapsed)
		{
			timeElapsed = _timeElapsed;
		};

	};


}
