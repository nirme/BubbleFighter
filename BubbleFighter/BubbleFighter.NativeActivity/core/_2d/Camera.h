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


		public:


		};

	}
}