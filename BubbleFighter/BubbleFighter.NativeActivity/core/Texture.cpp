#include "Texture.h"


namespace Graphic
{

	Texture2D::Texture2D(std::string _name, GLuint _id)
		: id(_id), name(_name)
	{};

	Texture2D::Texture2D(Texture2D&& t)
		: id(t.id), format(t.format), width(t.width), height(t.height)
	{
		name.swap(t.name);
		params.swap(t.params);

		t.id = 0;
		t.format = 0;
		t.width = 0;
		t.height = 0;
	};

	bool Texture2D::setParameter(GLenum pName, GLint pValue)
	{
		bool ret = false;
		for (int i = 0; i < params.size(); ++i)
		{

			if (params[i].first == pName)
			{
				ret = params[i].second != pValue;
				params[i].second = pValue;

				return ret;
			}
		}

		params.resize(params.size() + 1);
		params[params.size() - 1] = std::make_pair(pName, pValue);
		return true;
	};

	GLint Texture2D::getParameterByName(GLenum pName)
	{
		for (int i = 0; i < params.size(); ++i)
		{
			if (params[i].first == pName)
				return params[i].second;
		}
		return -1;
	};

	int Texture2D::getParametersNumber()
	{
		return params.size();
	};

	std::pair< GLenum, GLint> Texture2D::getParameterByIndex(unsigned int i)
	{
		//vectors throws on too big index
		return params.at(i);

	};

	const std::string& Texture2D::getName() const
	{
		return name;
	};


	typedef std::shared_ptr<Texture2D> PTexture2D;
}