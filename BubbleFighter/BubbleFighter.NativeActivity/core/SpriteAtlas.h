#pragma once

#include <string>
#include <map>
#include <vector>


#include <memory>


namespace core
{



	class Texture;
	typedef std::shared_ptr<Texture> PTexture;




	class SpriteAtlas
	{
	public:

		struct SpriteCoords
		{
			//  3---0
			//  |   |
			//  2---1
			union
			{
				float v[8];
				float points[4][2];
			};
		};


	protected:

		typedef std::vector<SpriteCoords> SpriteCoordsList;
		typedef std::map<std::string, unsigned short> SpriteCoordsMap;


		PTexture texture;
		SpriteCoordsList spriteCoordsList;
		SpriteCoordsMap spriteCoordsMap;




	public:

		void setTexture(PTexture& _texture)
		{
			texture = _texture;
		};






	};
}
