#pragma once

#include <string>
#include <memory>



namespace core
{

	class ResourceManager;




	typedef unsigned long int ResourceHandle;




	class Resource
	{
	protected:

		ResourceManager *manager;


		std::string name;
		std::string group;

		ResourceHandle handle;

		bool loaded;

		unsigned long int size;



		Resource();



	public:

		Resource(std::string _name, std::string _group, ResourceHandle _handle) :
			name(_name),
			group(_group),
			handle(_handle),
			loaded(false),
			size(0)
		{};

		virtual ~Resource()
		{};


		virtual void prepareImp() = 0;
		virtual void loadImp() = 0;

		virtual void unprepareImp() = 0;
		virtual void unloadImp() = 0;

		virtual int sizeCalcImpl() = 0;



		virtual int sizeCalc()
		{
			unsigned long int rsize = 0;

			rsize += sizeof(manager);

			rsize += name.size() * sizeof(char) + sizeof(std::string);
			rsize += group.size() * sizeof(char) + sizeof(std::string);

			rsize += sizeof(handle);
			rsize += sizeof(loaded);
			rsize += sizeof(size);

			rsize += sizeCalcImpl();

			return rsize;
		};



		virtual void load()
		{
			prepareImp();

			loadImp();

			loaded = true;

			size = sizeCalc();

		};



		virtual void unload()
		{
			loaded = false;

			unloadImp();

			unprepareImp();

			size = sizeCalc();

		};


	};



	typedef std::shared_ptr<Resource> ResourcePtr;

}
