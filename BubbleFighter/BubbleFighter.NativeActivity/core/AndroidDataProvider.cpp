#include "AndroidDataProvider.h"


namespace core
{

	AndroidDataProvider::AndroidDataProvider(AAssetManager *_assetManager, const std::string &_directory) :
		assetManager(_assetManager),
		directory(_directory)
	{};


	DataStreamPtr AndroidDataProvider::getDataStream(const std::string &_path)
	{
		assert(assetManager && "no asset manager set");

		std::string fullPath = directory + "/" + _path;
		AAsset *asset = AAssetManager_open(assetManager, fullPath.c_str(), AASSET_MODE_BUFFER);

		if (!asset)
		{
			std::string message = "Cannot find asset file '" + fullPath + "'";
			Logger::getSingleton().write(message, LL_CRITICAL);
		}

		return DataStreamPtr(new AndroidDataStream(asset));
	};

	void AndroidDataProvider::setDirectoryPath(const std::string &_dir)
	{
		directory = _dir;
	};

	DataProvider* AndroidDataProvider::clone()
	{
		return new AndroidDataProvider(assetManager, directory);
	};
}
