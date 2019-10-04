#pragma once


#include "SingletonTemplate.h"
#include "ScriptParser.h"

#include "Texture.h"


namespace core
{


	class ScriptLoader : public Singleton<ScriptLoader>
	{
	private:

		ScriptParserPtr parser;


	public:

		ScriptLoader() :
			parser(nullptr)
		{};


		const ScriptParser& getParser()
		{
			assert(parser && "no parser registered in script loader");
			return *parser.get();
		};

		void registerParser(ScriptParserPtr _parser)
		{
			parser = _parser;
		};

		ScriptNodeListPtr parse(DataStreamPtr _stream)
		{
			assert(parser && "no parser registered in script loader");
			return parser->parseScript(_stream);
		};



		TEXTURE_FILTER translateTextureFilter(const std::string& _filter)
		{
			if (_filter.compare("nearest"))
				return TF_NEAREST;

			if (_filter.compare("linear"))
				return TF_LINEAR;

			return (TEXTURE_FILTER)0;
		};




	};

}
