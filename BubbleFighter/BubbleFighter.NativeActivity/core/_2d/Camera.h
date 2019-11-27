#pragma once

#include "../Matrix3.h"
#include "SceneNode.h"

namespace core
{
	namespace _2d
	{

		class Camera : public SceneNode
		{
		
			mutable bool cashedViewMatrixNeedUpdate;
			mutable Matrix3 cashedViewMatrix;




			mutable bool cashedProjMatrixNeedUpdate;
			mutable Matrix3 cashedProjMatrix;



			virtual void _updateWorldTransform() const
			{
				SceneNode::_updateWorldTransform();
				cashedViewMatrixNeedUpdate = true;
			};


			virtual void _updateViewMatrix() const
			{
				assert(!cashedViewMatrixNeedUpdate && "Cashed matrix don't require updates");

				cashedViewMatrix = inverse(getWorldTransform());
				cashedViewMatrixNeedUpdate = false;
			};


		public:

			virtual const Matrix3& getViewMatrix()
			{
				if (!cashedViewMatrixNeedUpdate)
					_updateViewMatrix();

				return cashedViewMatrix;
			};

		};

	}
}