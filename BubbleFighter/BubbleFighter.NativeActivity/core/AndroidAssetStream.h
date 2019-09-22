#pragma once


#include "DataStream.h"


namespace core
{

	class AndroidDataStream : public DataStream
	{
	protected:
		AAsset* asset;


	public:

		AndroidDataStream(AAsset* _asset) :
			asset(_asset)
		{};

		virtual ~AndroidDataStream()
		{
			AAsset_close(asset);
		};


		virtual data_size getSize()
		{
			return (data_size) AAsset_getLength(asset);
		
		};


		virtual data_size seekPosition(data_size _pos)
		{
			off_t result = AAsset_seek(asset, (off_t)_pos, SEEK_SET);
			return (result == (off_t)-1) ? (data_size)-1 : result;
		};


		virtual data_size getPosition()
		{
			return AAsset_seek(asset, 0, SEEK_CUR);
		};


		virtual data_size readData(void * _buf, data_size _length)
		{
			assert(_buf && "read buffer cannot be nullptr");
			return AAsset_read(asset, _buf, _length);
		};

	};

}
