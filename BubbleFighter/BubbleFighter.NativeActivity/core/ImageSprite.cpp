#include "ImageSprite.h"

namespace core
{

	SpriteCoords::SpriteCoords(const Vector2 &_v0, const Vector2 &_v1, const Vector2 &_v2, const Vector2 &_v3, bool _inPixels) :
		uvPoints({ _v0, _v1, _v2, _v3 }), 
		inPixels(_inPixels)
	{};

	SpriteCoords::SpriteCoords(const SpriteCoords &_rhs) :
		uvPoints(_rhs.uvPoints),
		inPixels(_rhs.inPixels)
	{};

	SpriteCoords::SpriteCoords SPRITE_SQUARE = SpriteCoords(
		{ -0.5f, 0.5f },
		{ 0.5f, 0.5f },
		{ 0.5f, -0.5f },
		{ -0.5f, -0.5f }
	);

	bool SpriteCoords::toTextureSpace(unsigned int _w, unsigned int _h)
	{
		if (inPixels)
		{
			for (int i = 0; i < 4; ++i)
			{
				uvPoints[i].x /= _w;
				uvPoints[i].y = 1.0f - (uvPoints[i].y / _h);
			}

			inPixels = false;
			return true;
		}

		return false;
	};


	ImageSprite::ImageSprite(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager, TexturePtr _texture, const SpriteCoords &_coords) :
		Resource(_name, _handle, _group, _manager),
		texture(_texture),
		coords(_coords)
	{};

	void ImageSprite::loadImp()
	{
		if (!texture)
			texture = TextureManager::getSingleton().getByName(textureName, group);
		texture->load();

		if (coords.inPixels)
			coords.toTextureSpace(texture->getWidth(), texture->getHeight());
	};

	void ImageSprite::unloadImp()
	{
		texture.reset();
	};

	unsigned int ImageSprite::sizeCalcImpl()
	{
		unsigned int s = sizeof(SpriteCoords);
		s += sizeof(textureName) + textureName.size();
		return s;
	};


	void ImageSprite::setTexture(const std::string &_name)
	{
		textureName = _name;
	};

	void ImageSprite::setCoords(const SpriteCoords& _coords)
	{
		coords = _coords;
	};

}
