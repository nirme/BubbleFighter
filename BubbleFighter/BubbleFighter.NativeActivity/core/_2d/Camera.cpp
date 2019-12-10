#include "Camera.h"

namespace core
{
    namespace _2d
    {
        void Camera::_updateWorldTransform() const
        {
            SceneNode::_updateWorldTransform();
            cashedViewMatrixNeedUpdate = true;
        };


        void Camera::_updateViewMatrix() const
        {
            assert(!cashedViewMatrixNeedUpdate && "Cashed matrix don't require updates");

            cashedViewMatrix = inverse(getWorldTransform());
            cashedViewMatrixNeedUpdate = false;
        };


        const Matrix3& Camera::getViewMatrix() const
        {
            if (!cashedViewMatrixNeedUpdate)
                _updateViewMatrix();

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