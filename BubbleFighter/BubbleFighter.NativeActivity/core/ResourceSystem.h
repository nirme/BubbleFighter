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
			assert(dataProvider && "data provider not set");

			DataStreamPtr dataStream = dataProvider->getDataStream(_name);
			assert(dataStream && "configuration file not found");

			ScriptNodeListPtr configList = ScriptLoader::getSingleton().parse(dataStream);

			std::string name, path, file;
			for (auto it = configList->begin(); it != configList->end(); ++it)
			{
				name = (*it)->getValue("name");
				path = (*it)->getValue("resource_path");
				file = (*it)->getValue("script_file");
				
				auto itMan = resourceManagers.find(name);
				if (itMan != resourceManagers.end())
				{
					ResourceManager *manager = (*itMan).second;

					DataProviderPtr newDataProvider(dataProvider->clone());
					newDataProvider->setDirectoryPath(path);
					manager->registerDataProvider(newDataProvider);


					//  add parsing the base res script and forward nodes to manager

					DataStreamPtr resourceScriptStream = dataProvider->getDataStream(file);
					manager->parseResourceScript(resourceScriptStream);
				}
				else
				{
					std::string message = "resource manager by name '" + name + "' not registered";
					Logger::getSingleton().write(message, LL_CRITICAL);
				}
			}

		};


		void registerDataProvider(DataProviderPtr _dataProvider)
		{
			dataProvider = _dataProvider;
		};





	};

}

template<>
core::ResourceSystem* Singleton<core::ResourceSystem>::impl = nullptr;
