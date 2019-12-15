#pragma once

#include <string>
#include <cstdlib>

#include "Vector2.h"
#include "_2d/Quaternion.h"


#include "SingletonTemplate.h"

#include "ScriptParser.h"
#include "Texture.h"
#include "Shader.h"
#include "ImageSprite.h"
#include "SpritedFont.h"

#include "ScriptNode.h"
#include "DataStream.h"



namespace core
{


	class ScriptLoader : public Singleton<ScriptLoader>
	{
	private:
		ScriptParserPtr parser;

	public:

		ScriptLoader();


		const ScriptParser& getParser();

		void registerParser(ScriptParserPtr _parser);

		ScriptNodeListPtr parse(DataStreamPtr _stream);




		//  generic script value parsing


		std::string parseResourceName(ScriptNodePtr _node);
		std::string parseResourceGroup(ScriptNodePtr _node);


		//  texture specific script parsing

		TEXTURE_TYPE parseTextureType(ScriptNodePtr _node);
		TEXTURE_FILTER parseTextureFilter(ScriptNodePtr _node);


		//  shader specific script parsing

		SHADER_TYPE parseShaderType(ScriptNodePtr _node);
		std::string parseShaderName(ScriptNodePtr _node);

		ScriptNodePtr findProgramNode(ScriptNodeListPtr _nodeList, const std::string &_name);


		// sprite specific parsing

		ScriptNodePtr findSpriteNode(ScriptNodeListPtr _nodeList, const std::string &_name);
		std::string parseImgSpriteTexture(ScriptNodePtr _node);
		TextureSpriteCoords parseImgSpriteCoords(ScriptNodePtr _node);


		// sprited font

		std::string parseCodePoint(ScriptNodePtr _node);
		KerningPair parseKerningPair(ScriptNodePtr _node);
		FONT_SPACING parseFontSpacing(ScriptNodePtr _node);
		std::string parseFontTexture(ScriptNodePtr _node);
		float parseSizeMultiplier(ScriptNodePtr _node);
		float parseLineHeight(ScriptNodePtr _node);
		std::string parseDefaultChar(ScriptNodePtr _node);




		// scene node

		Vector2 parseNodeScale(ScriptNodePtr _node);
		_2d::Quaternion parseNodeRotation(ScriptNodePtr _node);
		Vector2 parseNodePosition(ScriptNodePtr _node);





		// object


		OBJECT_TYPE parseRenderableType(ScriptNodePtr _node)
		{
			std::string value = _node->getValue("object_type");

			if (value.compare("single_sprite") == 0)
				return OBJECT_TYPE::OT_SINGLE_SPRITE;

			return OBJECT_TYPE::OT_UNDEFINED;
		};


		std::string parseObjectName(ScriptNodePtr _node)
		{
			return _node->getValue("name");
		};

		std::string parseObjectShader(ScriptNodePtr _node)
		{
			return _node->getValue("shader_name");
		};

		std::string parseObjectImageSprite(ScriptNodePtr _node)
		{
			std::string firstSprite = _node->getValue("sprite_names");
			unsigned int pos = firstSprite.find_first_of(';');
			if (pos != firstSprite.npos)
				return firstSprite.substr(0, pos);
			return firstSprite;
		};

		Vector2 parseObjectScale(ScriptNodePtr _node)
		{
			std::string value = _node->getValue("scale");

			if (!value.size())
				return Vector2(1.0f);

			unsigned int pos = value.find_first_of(',');
			if (pos == value.npos)
				return Vector2(std::stof(value));

			return Vector2(std::stof(value), std::stof(value.substr(pos + 1)));
		};

		_2d::Quaternion parseObjectRotation(ScriptNodePtr _node)
		{
			std::string value = _node->getValue("rotation");
			return _2d::Quaternion(std::stof(value));
		};

		Vector2 parseObjectPosition(ScriptNodePtr _node)
		{
			std::string value = _node->getValue("position");
			unsigned int pos = value.find_first_of(',');
			if (!value.size() || pos == value.npos)
				return Vector2(0.0f);
			return Vector2(std::stof(value), std::stof(value.substr(pos + 1)));
		};

		_2d::Priority parseRenderablePriority(ScriptNodePtr _node)
		{
			std::string value = _node->getValue("render_priority");
			int iValue = std::stoi(value);
			
			if (iValue >= 0 && iValue <= std::numeric_limits<_2d::Priority>::max())
				return (_2d::Priority) iValue;

			if (iValue < 0)
				return 0;

			return std::numeric_limits<_2d::Priority>::max();
		};

		SpriteCoords parseSingleSpriteCoords(ScriptNodePtr _node)
		{
			std::string value = _node->getValue("sprite_coords");
			
			unsigned int posBegin = 0, posEnd = 0;
			std::vector<std::string> values;
			// tokenize

			do
			{
				posBegin = posEnd + 1;
				posEnd = value.find_first_of(',', posBegin);

				values.push_back(value.substr(posBegin, posEnd));
			} while(posEnd != value.npos);


			// left, right, top, bottom
			if (values.size() == 4)
				return SpriteCoords(
					std::stof(values[0]), 
					std::stof(values[1]), 
					std::stof(values[2]), 
					std::stof(values[3])
				);

			// v0, v1, v2, v3
			if (values.size() == 8)
				return SpriteCoords(
					Vector2(std::stof(values[0]), std::stof(values[1])), 
					Vector2(std::stof(values[2]), std::stof(values[3])), 
					Vector2(std::stof(values[4]), std::stof(values[5])), 
					Vector2(std::stof(values[6]), std::stof(values[7]))
				);

			// width, height
			if (values.size() == 2)
			{
				float width = std::stof(values[0]);
				float height = std::stof(values[1]);
				return SpriteCoords(width * -0.5f, width * 0.5f, height * 0.5f, height * -0.5f);
			}

			if (values.size() == 1)
			{
				float size = std::stof(values[0]);
				return SpriteCoords(size * -0.5f, size * 0.5f, size * 0.5f, size * -0.5f);
			}

			return SpriteCoords();
		};


	};

}
