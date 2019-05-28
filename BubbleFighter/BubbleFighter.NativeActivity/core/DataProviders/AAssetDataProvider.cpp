#include "AAssetDataProvider.h"



void AAssetDataProvider::setAAssetManager(AAssetManager* _aam_assetManager)
{
	aam_assetManager = _aam_assetManager;
};


AAssetDataProvider::AAssetDataProvider(AAssetManager* _aam_assetManager): IDataProvider(),aam_assetManager(_aam_assetManager)
{};


AAssetDataProvider::~AAssetDataProvider()
{};


const XmlData AAssetDataProvider::getConfig(const std::string& _str_configName)
{
	int i_size = 0;
	AAsset* aa_asset = AAssetManager_open(aam_assetManager,_str_configName.c_str(),AASSET_MODE_BUFFER);
	assert(aa_asset);

	i_size = AAsset_getLength(aa_asset);
	assert(i_size);

	char* c_data = new char[i_size+1];
	c_data[i_size] = 0; //null terminate the string
	AAsset_read(aa_asset,c_data,i_size);

	XmlDoc* xd_doc = new XmlDoc;
	xd_doc->parse< rapidxml::parse_no_string_terminators |
                   rapidxml::parse_no_entity_translation |
                   rapidxml::parse_no_utf8 |
                   rapidxml::parse_trim_whitespace >(c_data);

	AAsset_close(aa_asset);
	aa_asset = NULL;

	return XmlData(c_data,i_size,xd_doc);
};


const AssetData AAssetDataProvider::getAsset(const std::string& _str_assetName)
{
	int i_size = 0;
	AAsset* aa_asset = AAssetManager_open(aam_assetManager,_str_assetName.c_str(),AASSET_MODE_BUFFER);
	assert(aa_asset);

	i_size = AAsset_getLength(aa_asset);
	assert(i_size);

	unsigned char* uc_data = new unsigned char [i_size];

	AAsset_read(aa_asset,uc_data,i_size);
	AAsset_close(aa_asset);
	aa_asset = NULL;

	return AssetData(uc_data,i_size);
};
