#pragma once

#include "../Matrix3.h"
#include "SceneNode.h"

namespace core
{
	namespace _2d
	{

		class ViewPort
		{
			unsigned int width, height;

			Matrix3 projectionMatrix;

		public:

			virtual const Matrix3& getProjectionMatrix()
			{
			};


		};

	}
}