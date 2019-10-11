#include "TextureManager.h"



namespace core
{

	Resource* TextureManager::createImpl(std::string _name, ResourceHandle _handle, std::string _group, ScriptNodePtr _scriptNode)
	{
		Texture *texture = new Texture(_name, _handle, _group, this);
		texture->setFilter(ScriptLoader::getSingleton().parseTextureFilter(_scriptNode));
		return texture;
	};

	void TextureManager::removeImpl(ResourcePtr _resource)
	{
		_resource->unload();
	};


	TexturePtr TextureManager::getByName(const std::string& _name, const std::string& _group)
	{
		return std::static_pointer_cast<Texture>(getResourceByName(_name, _group));
	};


	TexturePtr TextureManager::getByHandle(ResourceHandle _handle)
	{
		return std::static_pointer_cast<Texture>(getResourceByHandle(_handle));
	};


	void TextureManager::parseResourceScript(DataStreamPtr _script)
	{
		ScriptLoader &loader = ScriptLoader::getSingleton();
		ScriptNodeListPtr nodeList = loader.parse(_script);

		std::string name, group, type, filter;

		for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			name = loader.parseResourceName(*it);
			group = loader.parseResourceGroup(*it);

			TexturePtr texture = std::static_pointer_cast<Texture>(createResource(name, group.length() == 0 ? DEFAULT_RESOURCE_GROUP : group, *it));

			texture->setType(loader.parseTextureType(*it));
			texture->setFilter(loader.parseTextureFilter(*it));
		}
	};

}
