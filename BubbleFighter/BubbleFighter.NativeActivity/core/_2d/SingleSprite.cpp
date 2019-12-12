#include "SingleSprite.h"


namespace core
{
	namespace _2d
	{
		
		SingleSprite::SingleSprite(Priority _renderPriority, MaterialPtr _material, ImageSpritePtr _sprite, SceneNode *_parent) :
			Renderable(_renderPriority, _material, true, _parent),
			sprite(_sprite)
		{};


		void SingleSprite::setResources(ShadingProgramPtr _program, ImageSpritePtr _sprite)
		{
			sprite = _sprite;
			setMaterial(_program, _sprite->getTexture());
		};


		BuffWriteResult SingleSprite::writeVertexData(GraphicBuffer &_buffer, unsigned int _fromSprite) const
		{
			assert(!_fromSprite || "_fromSprite must be 0 for SingleSprite");

			const Matrix3 &mx = getTransform();
			const TextureSpriteCoords &texCoords = sprite->getCoords();

			// 4 verts (x,y),(u,v)
			Vector2 vertices[8] = {
				TextureSpriteCoords::SPRITE_SQUARE.uvPoints[0],
				texCoords.uvPoints[0],
				TextureSpriteCoords::SPRITE_SQUARE.uvPoints[1],
				texCoords.uvPoints[1],
				TextureSpriteCoords::SPRITE_SQUARE.uvPoints[2],
				texCoords.uvPoints[2],
				TextureSpriteCoords::SPRITE_SQUARE.uvPoints[3],
				texCoords.uvPoints[3]
			};

			// correct ratio
			vertices[0].y *= texCoords.whRatio;
			vertices[2].y *= texCoords.whRatio;
			vertices[4].y *= texCoords.whRatio;
			vertices[6].y *= texCoords.whRatio;

			// and transform
			vertices[0] = transformPoint(mx, vertices[0]);
			vertices[2] = transformPoint(mx, vertices[2]);
			vertices[4] = transformPoint(mx, vertices[4]);
			vertices[6] = transformPoint(mx, vertices[6]);

			if (_buffer.write((float*)vertices, 8))
				return BuffWriteResult({1, true});

			return BuffWriteResult({ 0, false });
		};
	}
}
