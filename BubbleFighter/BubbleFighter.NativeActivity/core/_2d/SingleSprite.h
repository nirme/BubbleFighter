#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cstring>
#include "../SpriteCoord.h"

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
			MaterialPtr material; //this contains shader and texture already

		public:

			SingleSprite(Priority _renderPriority, MaterialPtr _material, ImageSpritePtr _sprite = nullptr, SceneNode *_parent = nullptr);

			void setResources(ShadingProgramPtr _program, ImageSpritePtr _sprite);

			virtual BuffWriteResult writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite = 0) const;


			virtual AxisAlignedBox getBoundingBox() const
			{
				float whRatio = sprite->getCoords().whRatio;

				// bottom left = uvPoints[3], top right = uvPoints[2]
				return AxisAlignedBox(
					SpriteCoords::SPRITE_SQUARE.uvPoints[3].x,
					SpriteCoords::SPRITE_SQUARE.uvPoints[3].y * whRatio,
					SpriteCoords::SPRITE_SQUARE.uvPoints[2].x,
					SpriteCoords::SPRITE_SQUARE.uvPoints[2].y * whRatio
				);
			};

		};
	}
}
