#pragma once

#include "Vector3.h"
#include "Matrix3.h"
#include "_2d/Renderable.h"
#include "_2d/MaterialManager.h"
#include "_2d/Camera.h"

namespace core
{
    /*
	namespace _2d
	{
		class Renderable;
		class Camera;
	}
     */

	class ShadingParamsPassthru
	{
		const _2d::Renderable *renderable;
		const _2d::Material *material;
		const _2d::Camera *camera;

		Vector3 ambientLight;
		float timeElapsed;


	public:

		ShadingParamsPassthru() :
		renderable(nullptr),
		material(nullptr),
		camera(nullptr),
		ambientLight({1.0f, 1.0f, 1.0f}),
		timeElapsed(0.0f)
		{};

		const Matrix3 &get2dWorldMatrix() const;
		const Matrix3 &get2dViewMatrix() const;

		Matrix3 get2dWorldViewMatrix() const;
		GLint getTextureId(unsigned int _index) const;

		const Vector3 &getAmbientLight() const;
		float getTimeElapsed() const;


		void setCurrentRenderable(const _2d::Renderable *_renderable);
		void setCurrentMaterial(const _2d::Material *_material);


		void setCamera(const _2d::Camera *_camera);

		void setAmbientLightColor(Vector3 _ambientLight);

		void setTimeElapsed(float _timeElapsed);

	};


}
