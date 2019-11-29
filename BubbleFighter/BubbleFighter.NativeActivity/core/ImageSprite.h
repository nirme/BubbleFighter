#pragma once

#include <math.h>

#include "Vector2.h"

#include "Resource.h"
#include "Texture.h"
#include "TextureManager.h"



namespace core
{
	struct SpriteCoords
	{
		union
		{
			float uvArray[8];
			Vector2 uvPoints[4];
		};

		float whRatio;
		bool inPixels;


		SpriteCoords(float _x0 = 1.0f, float _y0 = 0.0f, float _x1 = 1.0f, float _y1 = 1.0f, float _x2 = 0.0f, float _y2 = 1.0f, float _x3 = 0.0f, float _y3 = 0.0f, bool _inPixels = false);
		SpriteCoords(const Vector2 &_v0, const Vector2 &_v1, const Vector2 &_v2, const Vector2 &_v3, bool _inPixels = false);
		SpriteCoords(const SpriteCoords &_rhs);

		bool toTextureSpace(unsigned int _w, unsigned int _h);

		static const SpriteCoords SPRITE_SQUARE;
	};



	class ImageSprite : public Resource
	{
	protected:

		TexturePtr texture;

		SpriteCoords coords;


	public:

		ImageSprite(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager = nullptr, TexturePtr _texture = nullptr, const SpriteCoords &_coords = SpriteCoords());

		void loadImp();
		void unloadImp();

		unsigned int sizeCalcImpl();

		inline const SpriteCoords& getCoords() const
		{
			return coords;
		};

		inline TexturePtr getTexture() const
		{
			return texture;
		};

	};


	typedef std::shared_ptr<ImageSprite> ImageSpritePtr;

}
