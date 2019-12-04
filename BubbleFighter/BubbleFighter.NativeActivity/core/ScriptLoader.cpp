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

		if (filter.compare("linear") == 0)
			return TF_LINEAR;

		// default:
		//if (filter.compare("nearest") == 0)
		return TF_NEAREST;
	};


	//  shader specific script parsing

	SHADER_TYPE ScriptLoader::parseShaderType(ScriptNodePtr _node)
	{
		std::string filter = _node->getValue("type");

		if (filter.compare("vertex") == 0)
			return ST_VERTEX;

		if (filter.compare("fragment") == 0)
			return ST_FRAGMENT;

		return ST_UNKNOWN;
	};


	std::string ScriptLoader::parseShaderName(ScriptNodePtr _node)
	{
		return  _node->getValue("resource_name");
	};


	ScriptNodePtr ScriptLoader::findProgramNode(ScriptNodeListPtr _nodeList, const std::string &_name)
	{
		for (auto it = _nodeList->begin(), end = _nodeList->end(); it != end; ++it)
		{
			if (_name.compare((*it)->getValue("name")) == 0)
				return *it;
		}
		return nullptr;
	};


	// sprite specific parsing

	ScriptNodePtr ScriptLoader::findSpriteNode(ScriptNodeListPtr _nodeList, const std::string &_name)
	{
		for (auto it = _nodeList->begin(), end = _nodeList->end(); it != end; ++it)
		{
			if (_name.compare((*it)->getValue("name")) == 0)
				return *it;
		}
		return nullptr;
	};


	std::string ScriptLoader::parseImgSpriteTexture(ScriptNodePtr _node)
	{
		std::string texture = _node->getValue("name");

		std::size_t pos = texture.find_first_of('#');
		return pos != std::string::npos ? texture.substr(0, pos) : texture;
	};


	SpriteCoords ScriptLoader::parseImgSpriteCoords(ScriptNodePtr _node)
	{
		bool inPixel = _node->getValue("in_pixel").empty() ? false : true;

		std::string tmp = _node->getValue("top");
		float top = tmp.empty() ? 1.0f : std::stof(tmp);

		tmp = _node->getValue("left");
		float left = tmp.empty() ? 0.0f : std::stof(tmp);

		tmp = _node->getValue("bottom");
		float bottom = tmp.empty() ? 0.0f : std::stof(tmp);

		tmp = _node->getValue("right");
		float right = tmp.empty() ? 1.0f : std::stof(tmp);

		return SpriteCoords(left, right, top, bottom, inPixel);
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

		cp = _node->getValue("cp2").c_str();
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
