#pragma once

#include <string>
#include <vector>

#include "../Vector2.h"
#include "Quaternion.h"
#include "../Matrix3.h"

#include "../Math2D.h"
#include "AxisAlignedBox.h"

//#include "Camera.h"
//#include "RenderQueue.h"


namespace core
{
	namespace _2d
	{

		class Camera;
		class RenderQueue;


		class SceneNode
		{
		public:

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


			virtual AxisAlignedBox _boundingBoxImpl() const;
			void updateBoundingBox() const;
			virtual void _updateWorldTransform() const;
			virtual void _findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, AxisAlignedBox *_bounds) const;

		public:

			virtual ~SceneNode();

			inline void setParent(SceneNode *_parent)
			{
				parent = _parent;
			};

			inline SceneNode *getParent() const
			{
				return parent;
			};

			ChildNodeIterator getChildIterator(bool _end = false);
			void appendChild(SceneNode* _child);
			void removeChild(SceneNode* _child);
			const AxisAlignedBox& getBoundingBox() const;
			const Matrix3& getWorldTransform() const;
			void findVisibleRenderables(Camera *_camera, RenderQueue *_queue, AxisAlignedBox *_bounds) const;

		};

	}
}
