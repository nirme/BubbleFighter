#pragma once

#ifndef _I_DATA_PROVIDER
#define _I_DATA_PROVIDER

#include <string>

#include "AssetDataStruct.h"



class IDataProvider
{
public:
	IDataProvider(){};
	virtual ~IDataProvider(){};
	virtual const XmlData getConfig(const std::string& _str_configName) = 0;
	virtual const AssetData getAsset(const std::string& _str_assetName) = 0;
};


#endif //_I_DATA_PROVIDER
