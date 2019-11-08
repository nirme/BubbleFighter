#include "ShadingProgramManager.h"


template<>
core::ShadingProgramManager* Singleton<core::ShadingProgramManager>::impl = nullptr;

namespace core
{

	Resource* ShadingProgramManager::createImpl(std::string _name, ResourceHandle _handle, std::string _group, ScriptNodePtr _scriptNode)
	{
		Shader *shader = new Shader(_name, _handle, _group, this);
		shader->setType(ScriptLoader::getSingleton().parseShaderType(_scriptNode));
		return shader;
	};

	void ShadingProgramManager::removeImpl(ResourcePtr _resource)
	{
		_resource->unload();
	};


	ShadingProgramPtr ShadingProgramManager::getByName(const std::string& _name, const std::string& _group)
	{
		return std::static_pointer_cast<ShadingProgram>(getResourceByName(_name, _group));
	};

	ShadingProgramPtr ShadingProgramManager::getByHandle(ResourceHandle _handle)
	{
		return std::static_pointer_cast<ShadingProgram>(getResourceByHandle(_handle));
	};


	void ShadingProgramManager::parseResourceScript(DataStreamPtr _script)
	{
		ScriptLoader &loader = ScriptLoader::getSingleton();
		ScriptNodeListPtr nodeList = loader.parse(_script);

		std::string name, group, type, filter;

		for (auto it = nodeList->begin(); it != nodeList->end(); ++it)
		{
			name = loader.parseResourceName(*it);
			group = loader.parseResourceGroup(*it);

			ShadingProgramPtr program = std::static_pointer_cast<ShadingProgram>(createResource(name, group.length() == 0 ? DEFAULT_RESOURCE_GROUP : group, *it));

			std::list<std::string> shaders = loader.parseShaderList(*it);
			for (auto it = shaders.begin(); it != shaders.end(); ++it)
				program->setShader(*it);
		}
	};

}
