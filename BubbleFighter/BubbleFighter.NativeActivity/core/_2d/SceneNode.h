#pragma once

#include <string>
#include <vector>

#include "../Vector2.h"
#include "Quaternion.h"
#include "../Matrix3.h"

#include "AxisAlignedBox.h"
#include "MovableObject.h"


namespace core
{
	namespace _2d
	{

		class SceneNode
		{
			typedef std::vector<SceneNode*> ChildNodeList;
			typedef ChildNodeList::iterator ChildNodeIterator;
			typedef ChildNodeList::const_iterator ChildNodeConstIterator;

			typedef std::vector<MovableObject*> ObjectList;
			typedef ChildNodeList::iterator ObjectIterator;
			typedef ChildNodeList::const_iterator ObjectConstIterator;

		protected:


			std::string name;

			SceneNode* parent;
			ChildNodeList children;


			Vector2 scale;
			Quaternion rotation;
			Vector2 position;

			bool cashedTransformNeedUpdate;
			Matrix3 cashedWorldTransform;

			bool axisAlignedBoxNeedUpdate;
			AxisAlignedBox aaBox;


			ObjectList objects;


			void updateWorldTransform()
			{};



		public:


			void appendNode(SceneNode* _child)
			{

			};

			const AxisAlignedBox& getAxisAlignedBox()
			{
				return aaBox;
			};

			const AxisAlignedBox& getAxisAlignedBox()
			{
				return aaBox;
			};


			inline const Matrix3& getWorldTransform()
			{
				if (cashedTransformNeedUpdate)
					updateWorldTransform();

				return cashedWorldTransform;
			};



		};

	}
}
