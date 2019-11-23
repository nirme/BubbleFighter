#pragma once

#ifndef _AASSET_DATA_PROVIDER
#define _AASSET_DATA_PROVIDER

#include "IDataProvider.h"



/*
getResources().getIdentifier(name,"drawable", getPackageName())
http://blog.sephiroth.it/2010/10/24/reading-resource-files-from-native-code/
https://blog.rarepebble.com/android-resources-from-cpp/
AssetManager
http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/#comment-1850768990
https://stackoverflow.com/questions/22436259/android-ndk-why-is-aassetmanager-open-returning-null
http://mobilepearls.com/labs/native-android-api/#assets

https://www.codeproject.com/Articles/993067/Calling-Java-from-Cplusplus-with-JNI
https://stackoverflow.com/questions/12900695/how-to-obtain-jni-interface-pointer-jnienv-for-asynchronous-calls
https://calebfenton.github.io/2017/04/05/creating_java_vm_from_android_native_code/
https://forum.unity.com/threads/get-access-to-assetmanager-on-c-side.394432/
https://github.com/googlesamples/android-ndk/blob/master/native-audio/app/src/main/cpp/native-audio-jni.c
*/



//on release runtime we don't have any checks for any assets, those are small enough project to make sure all files are added in dev


class AAssetDataProvider: public IDataProvider
{
private:
	AAssetManager* aam_assetManager;
public:
	void setAAssetManager(AAssetManager* _aam_assetManager);


	AAssetDataProvider(AAssetManager* _aam_assetManager = NULL);
	~AAssetDataProvider();


	const XmlData getConfig(const std::string& _str_configName);
	const AssetData getAsset(const std::string& _str_assetName);
};


#endif //_AASSET_DATA_PROVIDER
