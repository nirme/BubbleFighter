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

			virtual void _findVisibleRenderablesImpl(Camera *_camera, RenderQueue *_queue, AxisAlignedBox *_bounds) const
			{};

		public:

			virtual ~SceneNode()
			{
				for (auto it = children.begin(); it != children.end(); ++it)
				{
					delete (*it);
					(*it) = nullptr;
				}
			};


			inline void setParent(SceneNode *_parent)
			{
				parent = _parent;
			};

			inline SceneNode *getParent() const
			{
				return parent;
			};

			ChildNodeIterator getChildIterator(bool _end = false)
			{
				return (!_end) ? children.begin() : children.end();
			};

			void appendChild(SceneNode* _child)
			{
				SceneNode *prevParent = _child->getParent();
				if (prevParent)
					prevParent->removeChild(_child);

				children.push_back(_child);
			};

			void removeChild(SceneNode* _child)
			{
				for (auto it = children.begin(); it != children.end(); ++it)
				{
					if (_child == (*it))
					{
						children.erase(it);
						_child->setParent(nullptr);
						return;
					}
				}
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


			void findVisibleRenderables(Camera *_camera, RenderQueue *_queue, AxisAlignedBox *_bounds) const
			{
				if (_bounds->isOverlapping(getBoundingBox()))
				{
					_findVisibleRenderablesImpl(_camera, _queue, _bounds);

					for (ChildNodeConstIterator it = children.begin(); it != children.end(); ++it)
						(*it)->findVisibleRenderables(_camera, _queue, _bounds);
				}

			};


		};

	}
}
