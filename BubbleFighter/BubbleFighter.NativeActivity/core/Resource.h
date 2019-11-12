#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <atomic>


namespace core
{

	class ResourceManager;

	typedef unsigned long int ResourceHandle;

	enum RESOURCE_STATE
	{
		RS_UNKNOWN = 0x00, 
		RS_UNLOADED = 0x01, 
		RS_LOADING = 0x02,
		RS_LOADED = 0x03,
		RS_UNLOADING = 0x04,
	};



	class Resource
	{
	protected:

		ResourceManager *manager;


		std::string name;
		std::string group;

		ResourceHandle handle;

		std::atomic<RESOURCE_STATE> state;

		unsigned long int size;



		Resource() :
			manager(nullptr), handle(0), state(RS_UNKNOWN), size(0)
		{};



	public:

		Resource(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager = nullptr);
		virtual ~Resource();


		inline const std::string& getName()
		{
			return name;
		};

		inline const std::string& getGroup()
		{
			return group;
		};

		inline ResourceHandle getHandle()
		{
			return handle;
		};

		inline bool isLoaded()
		{
			return state.load() == RS_LOADED;
		};

		inline unsigned long int getSize()
		{
			return size;
		};


		virtual void loadImp() = 0;

		virtual void unloadImp() = 0;

		virtual unsigned int sizeCalcImpl() = 0;

		virtual unsigned int sizeCalc();

		virtual void load();
		virtual void unload();

	};



	typedef std::shared_ptr<Resource> ResourcePtr;

}
