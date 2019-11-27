#pragma once

#include <string>
#include <cstdlib>

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
		std::list<std::string> parseShaderList(ScriptNodePtr _node);


		// sprite specific parsing

		std::string parseSpriteTexture(ScriptNodePtr _node);
		SpriteCoords parseSpriteCoords(ScriptNodePtr _node);


		// sprite specific parsing

		std::string parseSpriteTexture(ScriptNodePtr _node);
		SpriteCoords parseSpriteCoords(ScriptNodePtr _node);


		// sprited font

		std::string parseCodePoint(ScriptNodePtr _node);
		KerningPair parseKerningPair(ScriptNodePtr _node);
		FONT_SPACING parseFontSpacing(ScriptNodePtr _node);
		std::string parseFontTexture(ScriptNodePtr _node);
		float parseSizeMultiplier(ScriptNodePtr _node);
		float parseLineHeight(ScriptNodePtr _node);
		std::string parseDefaultChar(ScriptNodePtr _node);

	};

}
