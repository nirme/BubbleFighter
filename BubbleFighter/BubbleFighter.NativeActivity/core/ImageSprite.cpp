#include "ImageSprite.h"

namespace core
{

	SpriteCoords::SpriteCoords(float _left, float _right, float _top, float _bottom, bool _inPixels) :
		uvPoints({ {_left, _top}, {_right, _top}, {_left, _bottom}, {_right, _bottom} }),
		whRatio(fabsf((_top - _bottom) / (_right - _left))),
		inPixels(_inPixels)
	{};

	SpriteCoords::SpriteCoords(const Vector2 &_v0, const Vector2 &_v1, const Vector2 &_v2, const Vector2 &_v3, bool _inPixels) :
		uvPoints({ _v0, _v1, _v2, _v3 }), 
		whRatio(fabsf((_v3.y - _v0.y) / (_v3.x - _v0.x))),
		inPixels(_inPixels)
	{};

	SpriteCoords::SpriteCoords(const SpriteCoords &_rhs) :
		uvPoints(_rhs.uvPoints),
		whRatio(_rhs.whRatio),
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
		DataStreamPtr data = manager->openResource(ImageSprite::SpriteDefinitionFile);
		ScriptLoader &sloader = ScriptLoader::getSingleton();
		ScriptNodeListPtr spriteDataList = sloader.parse(data);

		ScriptNodePtr spriteData = sloader.findSpriteNode(spriteDataList, getName());

		std::string texName = sloader.parseImgSpriteTexture(spriteData);
		SpriteCoords spriteCoords = sloader.parseImgSpriteCoords(spriteData);

		texture = TextureManager::getSingleton().getByName(texName, getGroup());
		texture->load();

		if (spriteCoords.inPixels)
			spriteCoords.toTextureSpace(texture->getWidth(), texture->getHeight());

		coords = spriteCoords;
	};

	void ImageSprite::unloadImp()
	{
		texture.reset();
	};

	unsigned int ImageSprite::sizeCalcImpl()
	{
		unsigned int s = sizeof(SpriteCoords);
		return s;
	};

}
