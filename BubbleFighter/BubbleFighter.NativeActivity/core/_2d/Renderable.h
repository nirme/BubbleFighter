#pragma once

#include <memory>
#include "../Matrix3.h"
#include "../GraphicBuffer.h"
#include "Entity.h"
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

		class SceneNode;

		class Renderable
		{
		protected:

			Entity *parent;
			Priority renderPriority;
			MaterialPtr material;

			bool enabled;

			bool bufferable;


			void setMaterial(ShadingProgramPtr _program, TexturePtr _tex0, TexturePtr _tex1 = nullptr, TexturePtr _tex2 = nullptr, TexturePtr _tex3 = nullptr, TexturePtr _tex4 = nullptr, TexturePtr _tex5 = nullptr, TexturePtr _tex6 = nullptr, TexturePtr _tex7 = nullptr);


		public:

			Renderable(Priority _renderPriority, MaterialPtr _material, bool _enabled = true, bool _bufferable = true, Entity *_parent = nullptr);
			virtual ~Renderable();

			void changeParent(Entity *_parent);

			const Matrix3& getTransform() const;
			Priority getPriority() const;
			MaterialId getMaterialId() const;
			MaterialPtr getMaterial() const;

			void setPriority(Priority _renderPriority);

			void setEnabled(bool _enabled);
			bool isEnabled() const;

			// get bb for this renderable, not transformed
			virtual AxisAlignedBox getBoundingBox() const = 0;

			inline bool isBufferable() const
			{
				return bufferable;
			};

			virtual BuffWriteResult writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite = 0) const;

		};

	}
}
