#pragma once


namespace core
{
	typedef long int data_size;

	class DataStream
	{
	public:

		virtual ~DataStream() {};

		virtual data_size getSize() = 0;
		virtual data_size seekPosition(data_size _pos) = 0;
		virtual data_size getPosition() = 0;
		virtual data_size readData(void * _buf, data_size _length ) = 0;


		virtual data_size skip(data_size _length)
		{
			return seekPosition(getPosition() + _length);
		};

		virtual data_size getRemainingSize()
		{
			return getSize() - getPosition();
		};
	};

}