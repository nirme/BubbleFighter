#pragma once


#include <memory>

#include "SingletonTemplate.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Texture.h"



namespace core
{
	
	class TextureManager : public ResourceManager, public Singleton<TextureManager>
	{

	public:


		virtual Resource* createImpl(std::string _name, ResourceHandle _handle, std::string _group, ScriptNodePtr _scriptNode)
		{
			Texture *texture = new Texture(_name, _handle, _group, this);
			texture->setFilter(ScriptLoader::getSingleton().translateTextureFilter(_scriptNode->getAttribute("filter")));
			return texture;
		};

		virtual void removeImpl(ResourcePtr _resource)
		{
			_resource->unload();
		};


		TexturePtr getByName(const std::string& _name, const std::string& _group = DEFAULT_RESOURCE_GROUP)
		{
			return std::static_pointer_cast<Texture>(getResourceByName(_name, _group));
		};


		TexturePtr getByHandle(ResourceHandle _handle)
		{
			return std::static_pointer_cast<Texture>(getResourceByHandle(_handle));
		};





	};
}
