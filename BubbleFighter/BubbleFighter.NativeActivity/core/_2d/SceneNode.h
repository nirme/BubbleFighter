#pragma once

#include <string>
#include <vector>

#include "../Vector2.h"
#include "Quaternion.h"
#include "../Matrix3.h"

#include "../Math2D.h"
#include "AxisAlignedBox.h"

#include "Camera.h"
#include "RenderQueue.h"


namespace core
{
	namespace _2d
	{

		class SceneNode
		{
			typedef std::vector<SceneNode*> ChildNodeList;
			typedef ChildNodeList::iterator ChildNodeIterator;
			typedef ChildNodeList::const_iterator ChildNodeConstIterator;

		protected:


			std::string name;

			SceneNode* parent;
			ChildNodeList children;


			Vector2 scale;
			Quaternion rotation;
			Vector2 position;

			mutable bool cashedTransformNeedUpdate;
			mutable Matrix3 cashedWorldTransform;

			mutable bool boundingBoxNeedUpdate;
			mutable AxisAlignedBox aaBox;


			virtual AxisAlignedBox _boundingBoxImpl() const { return AxisAlignedBox(); };

			void updateBoundingBox() const
			{
				assert(!boundingBoxNeedUpdate && "Cashed bounding box don't require updates");

				AxisAlignedBox boundingBox = _boundingBoxImpl();

				if (children.size())
				{
					for (ChildNodeConstIterator it = children.begin(); it != children.end(); ++it)
						boundingBox.merge((*it)->getBoundingBox());
				}
				aaBox = boundingBox;

				boundingBoxNeedUpdate = true;
			};

			virtual void _updateWorldTransform() const
			{
				assert(!cashedTransformNeedUpdate && "Cashed matrix don't require updates");

				cashedWorldTransform = parent->getWorldTransform() * affine2DMatrix(scale, rotation, position);
				cashedTransformNeedUpdate = false;
			};

		public:


			void appendNode(SceneNode* _child)
			{

			};

			const AxisAlignedBox& getBoundingBox() const
			{
				if (!boundingBoxNeedUpdate)
					updateBoundingBox();

				return aaBox;
			};

			const Matrix3& getWorldTransform() const
			{
				if (cashedTransformNeedUpdate)
					_updateWorldTransform();

				return cashedWorldTransform;
			};


			virtual void findVisibleRenderables(Camera *_camera, RenderQueue *_queue, AxisAlignedBox *_bounds)
			{
				if (_bounds->isOverlapping(getBoundingBox()))
				{
					for (ChildNodeIterator it = children.begin(); it != children.end(); ++it)
						(*it)->findVisibleRenderables(_camera, _queue, _bounds);
				}

			};


		};

	}
}
