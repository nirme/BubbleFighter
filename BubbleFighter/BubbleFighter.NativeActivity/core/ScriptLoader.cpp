#include "ScriptLoader.h"


template<>
core::ScriptLoader* Singleton<core::ScriptLoader>::impl = nullptr;

namespace core
{
	ScriptLoader::ScriptLoader() :
		parser(nullptr)
	{};


	const ScriptParser& ScriptLoader::getParser()
	{
		assert(parser && "no parser registered in script loader");
		return *parser.get();
	};

	void ScriptLoader::registerParser(ScriptParserPtr _parser)
	{
		parser = _parser;
	};

	ScriptNodeListPtr ScriptLoader::parse(DataStreamPtr _stream)
	{
		assert(parser && "no parser registered in script loader");
		return parser->parseScript(_stream);
	};



	//  generic script value parsing


	std::string ScriptLoader::parseResourceName(ScriptNodePtr _node)
	{
		return _node->getValue("name");
	};


	std::string ScriptLoader::parseResourceGroup(ScriptNodePtr _node)
	{
		return _node->getValue("group");
	};


	//  texture specific script parsing


	TEXTURE_TYPE ScriptLoader::parseTextureType(ScriptNodePtr _node) // stub func, only 1 tex type allowed
	{
		std::string type = _node->getValue("type");

		// default
		//if (type.compare("texture2d") == 0)
		return TT_TEXTURE_2D;
	};


	TEXTURE_FILTER ScriptLoader::parseTextureFilter(ScriptNodePtr _node)
	{
		std::string filter = _node->getValue("filter");

		if (filter.compare("linear"))
			return TF_LINEAR;

		// default:
		//if (filter.compare("nearest")
		return TF_NEAREST;
	};


	//  shader specific script parsing

	SHADER_TYPE ScriptLoader::parseShaderType(ScriptNodePtr _node)
	{
		std::string filter = _node->getValue("type");

		if (filter.compare("vertex"))
			return ST_VERTEX;

		if (filter.compare("fragment"))
			return ST_FRAGMENT;

		// default:
		//if (filter.compare("nearest")
		return ST_UNKNOWN;
	};


	std::string ScriptLoader::parseShaderName(ScriptNodePtr _node)
	{
		return  _node->getValue("resource_name");
	};


	// sprite specific parsing

	std::string ScriptLoader::parseImgSpriteTexture(ScriptNodePtr _node)
	{
		return _node->getValue("texture_name");
	};


	SpriteCoords ScriptLoader::parseImgSpriteCoords(ScriptNodePtr _node)
	{
		SpriteCoords coords;

		std::string coordsInPixels = _node->getValue("sprite_coords_in_pixel");
		if (coordsInPixels.length() > 0)
			coords.inPixels = true;

		std::string coordsList = _node->getValue("sprite_coords");
		std::size_t pos(0);
		char *str(nullptr);

		for (int i = 0; i < 8; ++i)
		{
			pos = coordsList.find_first_of("0123456789.", pos);
			if (pos == std::string::npos)
				break;

			str = &coordsList[pos];
			coords.uvArray[i] = strtof(str, &str);

			pos = str - &coordsList[pos];
		}

		return coords;
	};


	// sprited font

	std::string ScriptLoader::parseCodePoint(ScriptNodePtr _node)
	{
		return _node->getValue("cp");
	};


	KerningPair ScriptLoader::parseKerningPair(ScriptNodePtr _node)
	{
		KerningPair value;

		std::string cp = _node->getValue("cp1").c_str();
		memcpy(value.cp1, cp.c_str(), cp.length() < 4 ? cp.length() : 4);

		std::string cp = _node->getValue("cp2").c_str();
		memcpy(value.cp2, cp.c_str(), cp.length() < 4 ? cp.length() : 4);

		value.kerning = std::stof(_node->getValue("kern"));

		return value;
	};


	FONT_SPACING ScriptLoader::parseFontSpacing(ScriptNodePtr _node)
	{
		std::string spacing = _node->getValue("spacing");

		if (spacing.compare("proportional") == 0)
			return FS_PROPORTIONAL;

		//if (spacing.compare("mono") == 0)
		return FS_MONOSPACE;
	};


	std::string ScriptLoader::parseFontTexture(ScriptNodePtr _node)
	{
		return _node->getValue("texture");
	};


	float ScriptLoader::parseSizeMultiplier(ScriptNodePtr _node)
	{
		return std::stof(_node->getValue("font_size"));
	};


	float ScriptLoader::parseLineHeight(ScriptNodePtr _node)
	{
		return std::stof(_node->getValue("line_height"));
	};


	std::string ScriptLoader::parseDefaultChar(ScriptNodePtr _node)
	{
		return _node->getValue("defaultChar");
	};


}
