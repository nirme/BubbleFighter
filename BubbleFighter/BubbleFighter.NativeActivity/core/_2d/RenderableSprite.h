#pragma once

#include <memory>
#include <vector>
#include <array>
#include <cstring>
#include "Renderable.h"
#include "../Texture.h"
#include "../Vector2.h"
#include "../math.h"


namespace core
{
	struct SpriteCoords
	{
		union
		{
			Vector2 uvPoints[4];
			float uvArray[8];
		};

		SpriteCoords(Vector2 _v0, Vector2 _v1, Vector2 _v2, Vector2 _v3) : uvPoints({ _v0, _v1, _v2, _v3 })
		{};

		static const SpriteCoords SPRITE_SQUARE;
	};

	class ImageSprite
	{
		std::string name;
		TexturePtr texture;
		SpriteCoords coords;
	public:



	};

	SpriteCoords SPRITE_SQUARE = SpriteCoords(
		{ -0.5f, 0.5f },
		{ 0.5f, 0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f }
	);



	typedef std::shared_ptr<ImageSprite> ImageSpritePtr;

	// SpritePanel ??

	namespace _2d
	{
		class RenderableSprite : public Renderable
		{
		protected:

			std::vector<ImageSpritePtr> sprites;


		public:

			RenderableSprite(Priority _renderPriority, MaterialPtr _material, SceneNode *_parent = nullptr) :
				Renderable(_renderPriority, _material, _parent, false)
			{};


/*
   0---1
1  | \ |  2
   2---3

   0,2,3
   0,3,1

*/

			virtual const VertexData* getSpriteVertexData(void *_buffer, unsigned int _bufferSize, unsigned int &_bytesWritten)
			{
				int vertSize = ((1 + sprites.size()) * sizeof(SpriteCoords));
				if (_bufferSize < vertSize)
				{
					_bytesWritten = 0;
					return nullptr;
				}

				const Matrix3 &transform = getTransform();

				SpriteCoords *spriteVerts = reinterpret_cast<SpriteCoords *>(_buffer);
				*spriteVerts = SpriteCoords::SPRITE_SQUARE;

				spriteVerts->uvPoints[0] = transformPoint(transform, spriteVerts->uvPoints[0]);
				spriteVerts->uvPoints[1] = transformPoint(transform, spriteVerts->uvPoints[1]);
				spriteVerts->uvPoints[2] = transformPoint(transform, spriteVerts->uvPoints[2]);
				spriteVerts->uvPoints[3] = transformPoint(transform, spriteVerts->uvPoints[3]);

				for (int i = 0; i < sprites.size(); ++i)
				{
					spriteVerts[i+1] = sprites[i]->getCoords();
				}

				_bufferSize = vertSize;
				return nullptr;
			};

		};
	}
}
