#pragma once


#include <memory>

#include "SingletonTemplate.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Texture.h"

#include "ScriptLoader.h"


namespace core
{
	
	class TextureManager : public ResourceManager, public Singleton<TextureManager>
	{

	public:

		virtual Resource* createImpl(std::string _name, ResourceHandle _handle, std::string _group, ScriptNodePtr _scriptNode);
		virtual void removeImpl(ResourcePtr _resource);

		TexturePtr getByName(const std::string& _name, const std::string& _group = DEFAULT_RESOURCE_GROUP);
		TexturePtr getByHandle(ResourceHandle _handle);

		void parseResourceScript(DataStreamPtr _script);

	};
}
