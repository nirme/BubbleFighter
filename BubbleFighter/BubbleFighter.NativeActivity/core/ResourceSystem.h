#pragma once

#include <unordered_map>
#include <vector>

#include "SingletonTemplate.h"
#include "ResourceManager.h"
#include "Logger.h"

#include "ScriptLoader.h"






namespace core
{
	class ResourceSystem : public Singleton<ResourceSystem>
	{
	private:

		typedef std::unordered_map<std::string, ResourceManager*> ResourceManagerMap;
		ResourceManagerMap resourceManagers;

		DataProviderPtr dataProvider;


	public:

		void registerResourceManager(const std::string& _resourceType, ResourceManager* _manager)
		{
			Logger::getSingleton().write("Registering resource manager of type " + _resourceType);
			resourceManagers[_resourceType] = _manager;
		};

		void unregisterResourceManager(const std::string& _resourceType)
		{
			resourceManagers.erase(_resourceType);
		};


		void parseConfiguration(const std::string& _name)
		{
			DataStreamPtr dataStream = dataProvider->getDataStream(_name);
			assert(dataStream && "Configuration file not found");

			ScriptNodeListPtr configList = ScriptLoader::getSingleton().parse(dataStream);

			std::string name, path, file;
			for (auto it = configList->begin(); it != configList->end(); ++it)
			{
				name = (**it).getAttribute("name");
				path = (**it).getAttribute("resource_path");
				file = (**it).getAttribute("script_file");
				
				auto it = resourceManagers.find(name);
				if (it != resourceManagers.end())
				{
					ResourceManager *manager = (*it).second;

					manager->registerDataProvider();
				}
				it
				
			}


		};

		const ScriptParser& 





	};
}
