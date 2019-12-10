#include "ViewPort.h"


namespace core
{
    namespace _2d
    {

        void ViewPort::_updateCash() const
        {
            cashedProjectionMatrix = Matrix3(((float)height / width) * scale * 0.5f, 0.0f, 0.0f, 0.0f, scale * 0.5f, 0.0f, 0.0f, 0.0f, 1.0f);

			Vector2 boundary(1.0f / cashedProjectionMatrix.m11, 1.0f / cashedProjectionMatrix.m22);

			cashedFrustrum.setMinimum(boundary * -1.0f);
			cashedFrustrum.setMaximum(boundary);

			cashNeedUpdate = false;
        };


        ViewPort::ViewPort() :
            width(1),
            height(1),
            scale(1.0f),
			cashNeedUpdate(true),
            cashedProjectionMatrix(Matrix3::IDENTITY)
        {};


        const Matrix3& ViewPort::getProjectionMatrix() const
        {
            if (cashNeedUpdate)
                _updateCash();

            return cashedProjectionMatrix;
        };


		const AxisAlignedBox& ViewPort::getFrustrum() const
		{
			if (cashNeedUpdate)
				_updateCash();

			return cashedFrustrum;
		};


		void ViewPort::setScreenSize(unsigned int _width, unsigned int _height)
        {
            width = _width;
            height = _height;
			cashNeedUpdate = true;
        };

        void ViewPort::setScale(float _scale)
        {
            scale = _scale;
        };



    }
}