#pragma once

#include <memory>
#include <limits>
#include <string>
#include <unordered_map>

#include "Resource.h"
#include "ImageSpriteManager.h"
#include "Texture.h"





namespace core
{
	struct CharacterSprite
	{
		union
		{
			Vector2 s[8];
			struct
			{
				Vector2 vert1;
				Vector2 tex1;
				Vector2 vert2;
				Vector2 tex2;
				Vector2 vert3;
				Vector2 tex3;
				Vector2 vert4;
				Vector2 tex4;
			};
		};
	};

	struct KerningPair
	{
		char cp1[4];
		char cp2[4];
		float kerning;
	};

	enum FONT_SPACING
	{
		FS_MONOSPACE = 0x00,
		FS_PROPORTIONAL = 0x01,
	};


	typedef std::vector<CharacterSprite> SpritedText;


	class SpritedFont : public Resource
	{
	public:
		typedef unsigned long int Character;

		static unsigned long int ToUtf8(Character _char, char *_out);
		static Character Utf8ToCodepoint(const char *_out);

	protected:

		constexpr static char* config = "configuration";
		constexpr static char* charMap = "characters";
		constexpr static char* kerning = "kerning";


		struct CharacterPair
		{
			Character first, second;
			CharacterPair(Character _first, Character _second);
			bool operator==(const CharacterPair &_rhs);
		};

		struct CharacterPairHash
		{
			size_t operator()(CharacterPair const& _pair) const noexcept;
		};


		typedef std::unordered_map<Character, ImageSpritePtr> CharacterSpritesMap;
		typedef std::unordered_map<CharacterPair, float, CharacterPairHash> KerningMap;

		FONT_SPACING spacing;

		TexturePtr spriteAtlas;

		CharacterSpritesMap characters;
		ImageSpritePtr defaultChar;
		KerningMap kerningTable;

		float sizeMultiplier;
		float lineHeight;


	public:

		void loadImp();
		void unloadImp();

		unsigned int sizeCalcImpl();

		SpritedText generateSpritedVector(const std::string &_text, float *_vectorWidth = nullptr, float *_vectorheight = nullptr, float _width = std::numeric_limits<double>::max());


	};

}


