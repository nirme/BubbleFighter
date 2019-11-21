#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cstring>
#include "Renderable.h"
#include "../Texture.h"
#include "../ImageSprite.h"
#include "../Vector2.h"
#include "../math.h"


namespace core
{
	namespace _2d
	{
		class SingleSprite : public Renderable
		{
		protected:

			ImageSpritePtr sprite;


		public:

			SingleSprite(Priority _renderPriority, MaterialPtr _material, ImageSpritePtr _sprite = nullptr, SceneNode *_parent = nullptr);

			void setResources(ShadingProgramPtr _program, ImageSpritePtr _sprite);

			virtual BuffWriteResult writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite = 0) const;
		};
	}
}
