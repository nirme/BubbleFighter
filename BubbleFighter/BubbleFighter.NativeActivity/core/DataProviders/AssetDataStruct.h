#pragma once

#ifndef _ASSET_DATA_PROVIDER
#define _ASSET_DATA_PROVIDER


#include <memory>
#include "utils/rapidxml_wrapper.h"



	class XmlData
	{
	public:

		std::shared_ptr<const char> c_data;
		long l_dataSize;

		std::shared_ptr<const XmlNode> xn_root;

		XmlData();

		XmlData(const char* _c_data, long _l_dataSize, const XmlNode* _xn_root);
		XmlData(const XmlData& _rhs);
		XmlData(XmlData&& _rhs);

		XmlData& operator=(const XmlData& _rhs);
		XmlData& operator=(XmlData&& _rhs);
	};



	class AssetData
	{
	public:

		std::shared_ptr<const unsigned char> uc_data;
		long l_dataSize;


		AssetData();

		AssetData(const unsigned char* _uc_data, long _l_dataSize);
		AssetData(const AssetData& _rhs);
		AssetData(AssetData&& _rhs);

		AssetData& operator=(const AssetData& _rhs);
		AssetData& operator=(AssetData&& _rhs);
	};


#endif //_ASSET_DATA_PROVIDER
