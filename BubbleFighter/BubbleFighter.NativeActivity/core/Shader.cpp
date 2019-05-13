#include "Shader.h"

namespace Graphic
{
	Shader::Shader(std::string _name, GLuint _id, GLenum _type)
		: name(_name), id(_id), type(_type)
	{};

	Shader::Shader(Shader&& s)
		: id(s.id), type(s.type)
	{
		name.swap(s.name);

		s.id = 0;
		s.type = 0;
	};


	std::string Shader::getName() const
	{
		return name;
	};

	GLenum Shader::getType() const
	{
		return type;
	};

}



