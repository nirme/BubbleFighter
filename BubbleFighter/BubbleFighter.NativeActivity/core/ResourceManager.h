#pragma once

#include <unordered_map>
#include <map>
#include <queue>
#include <string>


#include "Resource.h"


namespace core
{
	constexpr auto DEFAULT_RESOURCE_GROUP = "";

	class ResourceManager
	{
	private:

		inline ResourceHandle getFreeHandle()
		{
			return resourceHandleMap.size() > 0 ? resourceHandleMap.rbegin()->first + 1 : 0;
		};

		inline bool isDefaultGroup(const std::string& _group)
		{
			return _group.length() == 0;
		};


	protected:

		typedef std::unordered_map<std::string, ResourcePtr> ResourceNameMap;
		typedef std::unordered_map<std::string, ResourceNameMap> ResourceGroupMap;

		// change it to vector maybe?
		typedef std::map<ResourceHandle, ResourcePtr> ResourceHandleMap;


		ResourceNameMap globalResourceMap;
		ResourceGroupMap groupedResourceMaps;

		ResourceHandleMap resourceHandleMap;

		std::queue<ResourcePtr> loadingResources;


	public:

		ResourcePtr getResourceByName(const std::string& _name, const std::string& _group = DEFAULT_RESOURCE_GROUP)
		{

			if (isDefaultGroup(_group))
			{
				auto it = globalResourceMap.find(_name);
				if (it != globalResourceMap.end())
				{
					return it->second;
				}
			}
			else
			{
				auto resourceMapIt = groupedResourceMaps.find(_group);
				if (resourceMapIt != groupedResourceMaps.end())
				{
					auto it = resourceMapIt->second.find(_name);
					if (it != resourceMapIt->second.end())
					{
						return it->second;
					}
				}
			}


			//resource not found in specified group or group not found, look for it in all groups

			// check in global
			{
				auto it = globalResourceMap.find(_name);
				if (it != globalResourceMap.end())
				{
					return it->second;
				}
			}

			//check in all grouped lists
			for (auto resourceMapIt = groupedResourceMaps.begin(); resourceMapIt != groupedResourceMaps.end(); ++resourceMapIt)
			{
				auto it = resourceMapIt->second.find(_name);
				if (it != resourceMapIt->second.end())
				{
					return it->second;
				}
			}

			return ResourcePtr();
		};

		ResourcePtr getResourceByHandle(ResourceHandle _handle)
		{
			auto it = resourceHandleMap.find(_handle);
			return it != resourceHandleMap.end() ? it->second : ResourcePtr();
		};



	};


}