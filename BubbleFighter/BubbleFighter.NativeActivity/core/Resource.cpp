#include "Resource.h"


namespace core
{
	//Resource::


	Resource::Resource(std::string _name, ResourceHandle _handle, std::string _group, ResourceManager *_manager) :
		manager(_manager),
		name(_name),
		group(_group),
		handle(_handle),
		state(RS_UNLOADED),
		size(0)
	{};

	Resource::~Resource()
	{};


	unsigned int Resource::sizeCalc()
	{
		unsigned long int rsize = 0;

		rsize += sizeof(manager);

		rsize += name.size() * sizeof(char) + sizeof(std::string);
		rsize += group.size() * sizeof(char) + sizeof(std::string);

		rsize += sizeof(handle);
		rsize += sizeof(state);
		rsize += sizeof(size);

		rsize += sizeCalcImpl();

		return rsize;
	};



	void Resource::load()
	{
		assert(state.load() != RS_UNKNOWN && "cannot load uninitialised resource");

		//  wait if already loading in background
		while (state.load() == RS_LOADING);

		if (state.load() == RS_LOADED) return;

		//  wwait in case it's being unloaded
		while (state.load() != RS_UNLOADED);

		state = RS_LOADING;

		loadImp();

		state = RS_LOADED;

		size = sizeCalc();
	};


	void Resource::unload()
	{
		assert(state.load() != RS_UNKNOWN && "cannot unload uninitialised resource");

		while (state.load() == RS_UNLOADING);

		if (state.load() == RS_UNLOADED) return;

		while (state.load() == RS_LOADED);

		state = RS_UNLOADING;

		unloadImp();

		state = RS_UNLOADED;

		size = sizeCalc();

	};

}
