#pragma once

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

		bool inPixels;

		SpriteCoords(const Vector2 &_v0 = Vector2(1.0f, 0.0f), 
					const Vector2 &_v1 = Vector2(1.0f, 1.0f), 
					const Vector2 &_v2 = Vector2(0.0f, 1.0f), 
					const Vector2 &_v3 = Vector2(0.0f, 0.0f), 
					bool _inPixels = false);

		SpriteCoords(const SpriteCoords &_rhs);

		bool toTextureSpace(unsigned int _w, unsigned int _h);

		static const SpriteCoords SPRITE_SQUARE;
	};



	class ImageSprite : public Resource
	{
	protected:

		std::string textureName;
		TexturePtr texture;

		SpriteCoords coords;


	public:

		ImageSprite(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager = nullptr, TexturePtr _texture = nullptr, const SpriteCoords &_coords = SpriteCoords());

		void loadImp();
		void unloadImp();

		unsigned int sizeCalcImpl();

		void setTexture(const std::string &_name);
		void setCoords(const SpriteCoords& _coords);


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
