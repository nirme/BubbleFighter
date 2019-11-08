#pragma once

#include <memory>

#include "SingletonTemplate.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Shader.h"
#include "ScriptLoader.h"


namespace core
{

	class ShaderManager : public ResourceManager, public Singleton<ShaderManager>
	{

	public:

		virtual Resource* createImpl(std::string _name, ResourceHandle _handle, std::string _group, ScriptNodePtr _scriptNode);
		virtual void removeImpl(ResourcePtr _resource);

		ShaderPtr getByName(const std::string& _name, const std::string& _group = DEFAULT_RESOURCE_GROUP);
		ShaderPtr getByHandle(ResourceHandle _handle);

		void parseResourceScript(DataStreamPtr _script);

	};
}
