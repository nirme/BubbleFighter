#pragma once

#include <memory>
#include "../Matrix3.h"
#include "../GraphicBuffer.h"
#include "SceneNode.h"
#include "MaterialManager.h"
#include "AxisAlignedBox.h"


namespace core
{
	namespace _2d
	{
		typedef unsigned char Priority;

		struct BuffWriteResult
		{
			//index of sprite for next write operation in case last write was only partial
			unsigned int nextSpriteIndex;

			//true if all data was writen to buffer (nextSpriteIndex=0)
			bool operComplete;
		};

		class Renderable
		{
		protected:

			SceneNode *parent;
			Priority renderPriority;
			MaterialPtr material;

			bool enabled;

			bool bufferable;


		public:

			Renderable(Priority _renderPriority, MaterialPtr _material, bool _enabled = true, bool _bufferable = true, SceneNode *_parent = nullptr);
			virtual ~Renderable();

			void changeParent(SceneNode *_parent);

			inline const Matrix3& getTransform() const
			{
				return parent ? parent->getWorldTransform() : Matrix3::IDENTITY;
			};

			inline Priority getPriority() const
			{
				return renderPriority;
			};

			inline MaterialId getMaterialId() const
			{
				return material->id;
			};

			
			inline void setPriority(Priority _renderPriority)
			{
				renderPriority = _renderPriority;
			};

			void setMaterial(ShadingProgramPtr _program, TexturePtr _texture);


			void setEnabled(bool _enabled)
			{
				enabled = _enabled;
			};

			// get bb for this renderable, not transformed
			virtual AxisAlignedBox getBoundingBox() = 0;

			inline bool isBufferable() const
			{
				return bufferable;
			};

			virtual BuffWriteResult writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite = 0) const;



			class Visitor
			{
			public:
				virtual ~Visitor() {};
				virtual void visit(Renderable *_renderable) = 0;
			};
		};

	}
}
