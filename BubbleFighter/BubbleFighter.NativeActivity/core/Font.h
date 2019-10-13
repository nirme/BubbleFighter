#pragma once

#include <memory>
#include <unordered_map>

#include "Resource.h"
#include "Texture.h"





namespace core
{

	enum FONT_TYPE
	{
		FT_SPRITED = 0x00,
	};

	enum FONT_SPACING
	{
		FS_MONOSPACE = 0x00,
		FS_PROPORTIONAL = 0x01,
	};


	class Font : public Resource
	{
	private:
		typedef unsigned short Character;
		struct CharacterPair
		{
			union
			{
				struct {
					Character first, second;
				};
				unsigned long int key;
			};

			bool operator ==(const CharacterPair &_rhs) const
			{
				return key == _rhs.key;
			};

			size_t operator()() const
			{
				return key;
			}

		};

		struct SpriteCoords
		{
			float left;
			float top;
			float right;
			float bottom;
		};

		typedef std::unordered_map<Character, SpriteCoords> CharacterMap;
		typedef std::unordered_map<CharacterPair, float> KerningMap;




		FONT_TYPE type;
		FONT_SPACING spacing;
		bool usesKerning;

		TexturePtr sprite;

		CharacterMap characters;
		KerningMap kerning;


	};

}
