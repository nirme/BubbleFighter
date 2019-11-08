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


	std::list<std::string> ScriptLoader::parseShaderList(ScriptNodePtr _node)
	{
		ScriptNodeListPtr list = _node->getChildList();
		std::list<std::string> shaders;

		for (auto it = list->begin(); it != list->end(); ++it)
		{
			if ((*it)->getName().compare("shader"))
				shaders.push_back((*it)->getValue("name"));
		}

		return shaders;
	};



}
