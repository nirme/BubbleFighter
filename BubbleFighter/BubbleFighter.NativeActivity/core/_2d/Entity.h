#pragma once

#include "SceneNode.h"
#include "Renderable.h"
#include "../Math2D.h"



namespace core
{
	namespace _2d
	{

		class Entity : public SceneNode
		{
			typedef std::vector<Renderable*> RenderableList;
			typedef RenderableList::iterator RenderableIterator;
			typedef RenderableList::const_iterator RenderableConstIterator;

		protected:



			RenderableList renderables;


			virtual AxisAlignedBox _boundingBoxImpl() const
			{
				return AxisAlignedBox();
			};



			virtual AxisAlignedBox _boundingBoxImpl() const
			{
				AxisAlignedBox bb;

				const Matrix3& mx = getWorldTransform();
				for (RenderableConstIterator it = renderables.begin(); it != renderables.end(); ++it)
					bb.merge((*it)->getBoundingBox().transform(mx));

				return bb;
			};


		public:


		};

	}
}
