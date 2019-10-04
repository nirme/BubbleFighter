#pragma once

#include <memory>
#include "DataStream.h"

namespace core
{

	class DataProvider
	{



	public:
		virtual DataStreamPtr getDataStream(const std::string& _path) = 0;

		virtual void setDirectionPath(const std::string& _path) = 0;

	};



	typedef std::shared_ptr<DataProvider> DataProviderPtr;
}
