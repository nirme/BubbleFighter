#pragma once

#include <memory>
#include <vector>
#include <array>
#include "Renderable.h"
#include "../Vector2.h"


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
		class Sprite : public Renderable
		{
		protected:

			std::vector<ImageSpritePtr> sprites;


		public:

			Sprite(Priority _renderPriority, MaterialPtr _material, SceneNode *_parent = nullptr) :
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
				if (_bufferSize < ((1 + sprites.size()) * sizeof(float)))
				{
					_bytesWritten = 0;
					return nullptr;
				}

				const Matrix3 &transform = getTransform();


				

				float *vertArray
				_buffer
				_buffer
				return nullptr;
			};

		};
	}
}
