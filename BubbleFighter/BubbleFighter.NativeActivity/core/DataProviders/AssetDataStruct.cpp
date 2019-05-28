#include "AssetDataStruct.h"







//std::shared_ptr<const char> c_data;
//long l_dataSize;
//std::shared_ptr<const XmlNode> xn_root;

XmlData::XmlData() : 
	c_data(std::nullptr_t()),
	l_dataSize(0),
	xn_root(std::nullptr_t())
{};


XmlData::XmlData(const char* _c_data,long _l_dataSize,const XmlNode* _xn_root) :
	c_data(_c_data,std::default_delete<const char[]>()),
	l_dataSize(_l_dataSize),
	xn_root(_xn_root)
{};



XmlData::XmlData(const XmlData& _rhs) :
	c_data(_rhs.c_data),
	l_dataSize(_rhs.l_dataSize),
	xn_root(_rhs.xn_root)
{};


XmlData::XmlData(XmlData&& _rhs):
	c_data(_rhs.c_data),
	l_dataSize(_rhs.l_dataSize),
	xn_root(_rhs.xn_root)
{};


XmlData& XmlData::operator=(const XmlData& _rhs)
{
	c_data = _rhs.c_data;
	l_dataSize = _rhs.l_dataSize;
	xn_root = _rhs.xn_root;
	return *this;
};


XmlData& XmlData::operator=(XmlData&& _rhs)
{
	c_data = _rhs.c_data;
	l_dataSize = _rhs.l_dataSize;
	xn_root = _rhs.xn_root;
	return *this;
};



AssetData::AssetData() : 
	uc_data(std::nullptr_t()), 
	l_dataSize(0)
{};


AssetData::AssetData(const unsigned char* _uc_data,long _l_dataSize) :
	uc_data(_uc_data,std::default_delete<const unsigned char[]>()),
	l_dataSize(_l_dataSize)
{};


AssetData::AssetData(const AssetData& _rhs) :
	uc_data(_rhs.uc_data), 
	l_dataSize(_rhs.l_dataSize)
{};


AssetData::AssetData(AssetData&& _rhs):
	uc_data(_rhs.uc_data),
	l_dataSize(_rhs.l_dataSize)
{};


AssetData& AssetData::operator=(const AssetData& _rhs)
{
	uc_data = _rhs.uc_data;
	l_dataSize = _rhs.l_dataSize;
	return *this;
};


AssetData& AssetData::operator=(AssetData&& _rhs)
{
	uc_data = _rhs.uc_data;
	l_dataSize = _rhs.l_dataSize;
	return *this;
};
