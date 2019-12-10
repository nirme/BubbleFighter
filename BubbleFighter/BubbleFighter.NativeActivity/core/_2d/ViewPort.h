#pragma once

#include "../Matrix3.h"
#include "AxisAlignedBox.h"

namespace core
{
	namespace _2d
	{



		class ViewPort
		{
			unsigned int width, height;
			float scale;

			mutable bool cashNeedUpdate;
			mutable Matrix3 cashedProjectionMatrix;
			mutable AxisAlignedBox cashedFrustrum;


			void _updateCash() const;

		public:

			ViewPort();

			virtual const Matrix3& getProjectionMatrix() const;
			virtual const AxisAlignedBox& getFrustrum() const;

			void setScreenSize(unsigned int _width, unsigned int _height);
			void setScale(float _scale);

		};

	}
}