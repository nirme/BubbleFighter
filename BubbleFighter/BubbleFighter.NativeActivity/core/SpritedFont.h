#pragma once

#include <memory>
#include <limits>
#include <unordered_map>

#include "Resource.h"
#include "ImageSprite.h"
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

	class SpritedFont : public Resource
	{
	public:
		typedef unsigned long int Character;

		enum FONT_SPACING
		{
			FS_MONOSPACE = 0x00,
			FS_PROPORTIONAL = 0x01,
		};

	protected:

		struct CharacterPair
		{
			Character first, second;
			CharacterPair(Character _first, Character _second) : first(_first), second(_second)
			{};

			bool operator==(const CharacterPair &_rhs)
			{
				return first == _rhs.first && second == _rhs.second;
			};
		};

		struct CharacterPairHash
		{
			size_t operator()(CharacterPair const& _pair) const noexcept
			{
				return (size_t)(_pair.first ^ _pair.second);
			}
		};


		typedef std::unordered_map<Character, ImageSpritePtr> CharacterSpritesMap;
		typedef std::unordered_map<CharacterPair, float, CharacterPairHash> KerningMap;

		FONT_SPACING spacing;

		TexturePtr spriteAtlas;

		CharacterSpritesMap characters;
		ImageSpritePtr defaultChar;
		KerningMap kerning;

		float sizeMultiplier;
		float lineHeight;


	public:

		void loadImp()
		{};

		void unloadImp()
		{};

		unsigned int sizeCalcImpl()
		{};



		std::vector<CharacterSprite> generateSpritedVector(const std::string &_text, float *_vectorWidth = nullptr, float *_vectorheight = nullptr, float _width = std::numeric_limits<double>::max())
		{
			unsigned long int len = _text.length();
			if (!len)
				return std::vector<CharacterSprite>();

			const char* textArr = _text.c_str();

			Vector2 nextLetterPosition;
			Vector2 spriteSize;
			Vector2 textSize;

			CharacterSpritesMap::iterator charSprite;
			KerningMap::iterator kern;
			ImageSpritePtr sprite;

			std::vector<CharacterSprite> out(len);

			for (unsigned long int i = 0; i < len; ++i)
			{
				charSprite = characters.find((Character)textArr[i]);
				sprite = charSprite != characters.end() ? (*charSprite).second : defaultChar;

				const SpriteCoords &texCoords = sprite->getCoords();
				//fill in texels
				out[i].tex1 = texCoords.uvPoints[0];
				out[i].tex2 = texCoords.uvPoints[1];
				out[i].tex3 = texCoords.uvPoints[2];
				out[i].tex4 = texCoords.uvPoints[3];


				spriteSize = Vector2(
					sizeMultiplier * (texCoords.uvPoints[1].x - texCoords.uvPoints[0].x),
					sizeMultiplier * (texCoords.uvPoints[0].y - texCoords.uvPoints[2].y)
				);

				if (nextLetterPosition.x + spriteSize.x > _width)
				{
					//update text width
					if (out[i - 1].vert2.x > textSize.x)
						textSize.x = out[i - 1].vert2.x;

					nextLetterPosition.x = 0.0f;
					nextLetterPosition.y -= lineHeight;
				}

				//fill in vertices
				out[i].vert1 = nextLetterPosition;

				out[i].vert2 = nextLetterPosition;
				out[i].vert2.x += spriteSize.x;

				out[i].vert3 = nextLetterPosition;
				out[i].vert3.y += spriteSize.y;

				out[i].vert4 = nextLetterPosition + spriteSize;


				nextLetterPosition.x += spriteSize.x;
				if (spacing == FONT_SPACING::FS_PROPORTIONAL && i + 1 < len)
				{
					kern = kerning.find(CharacterPair((Character)textArr[i], (Character)textArr[i+1]));
					if (kern != kerning.end())
						nextLetterPosition.x += (*kern).second;
				}
			}


			if (out.back().vert4.x > textSize.x)
				textSize.x = out.back().vert4.x;

			textSize.y = -out.back().vert4.y;
			
			if (_vectorWidth)
				*_vectorWidth = textSize.x;

			if (_vectorheight)
				*_vectorheight = textSize.y;

			return out;
		};


	};

}


