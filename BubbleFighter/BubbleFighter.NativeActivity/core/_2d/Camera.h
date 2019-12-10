#pragma once

#include "../Matrix3.h"
#include "../Math2D.h"
#include "SceneNode.h"
#include "ViewPort.h"

namespace core
{
	namespace _2d
	{

		class Camera : public SceneNode
		{
			ViewPort* viewPort;
		
			mutable bool cashedViewMatrixNeedUpdate;
			mutable Matrix3 cashedViewMatrix;

			mutable bool cashedProjMatrixNeedUpdate;
			mutable Matrix3 cashedProjMatrix;


			virtual void _updateWorldTransform() const;
			virtual void _updateViewMatrix() const;

		public:

			virtual const Matrix3& getViewMatrix() const;

			virtual void setViewPort(ViewPort* _viewPort);
			virtual ViewPort* getViewPort() const;

		};

	}
}