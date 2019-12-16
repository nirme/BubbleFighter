#include "Camera.h"

namespace core
{
    namespace _2d
    {

		void Camera::_invalidateWorldTransformImpl() const
		{
			cashedViewMatrixNeedUpdate = false;
		};


		Camera::Camera(const char *_name, ViewPort* _viewPort) :
			MovableObject(_name),
			viewPort(_viewPort),
			cashedViewMatrixNeedUpdate(true)
		{};


		const Matrix3& Camera::getViewMatrix() const
		{
			if (cashedViewMatrixNeedUpdate)
			{
				cashedViewMatrix = inverse(getWorldTransform());
				cashedViewMatrixNeedUpdate = false;
			}

			return cashedViewMatrix;
		};


		void Camera::setViewPort(ViewPort* _viewPort)
		{
			viewPort = _viewPort;
		};


		ViewPort* Camera::getViewPort() const
		{
			return viewPort;
		};

    }
}