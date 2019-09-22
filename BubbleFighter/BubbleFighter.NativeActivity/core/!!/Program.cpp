#include "Program.h"

namespace Graphic
{
	
	Program::Program(std::string _name, GLuint _id)
		: name(_name), id(_id)
	{};

	Program::Program(Program&& p)
		: id(p.id)
	{
		name.swap(p.name);
		attachedShaders.swap(p.attachedShaders);
		uniformLocations.swap(p.uniformLocations);

		p.id = 0;
	};

	std::string Program::getName() const
	{
		return name;
	};

	void Program::setAttachedShader(PShader shader)
	{
		GLenum type = shader->getType();

		for (auto it = attachedShaders.begin(); it != attachedShaders.end(); ++it)
		{
			if ((*it).first == type)
			{
				(*it).second = shader;
				return;
			}
		}

		attachedShaders.push_back(std::make_pair(type, shader));
	};

	void Program::setUniformId(const std::string& uniformName, GLint uniformId)
	{
		auto it = uniformLocations.find(uniformName);
		if (it != uniformLocations.end())
			(*it).second = uniformId;
		else
			uniformLocations.insert(std::make_pair(uniformName, uniformId));
	};

	GLint Program::getUniformId(const std::string& uniformName)
	{
		auto it = uniformLocations.find(uniformName);
		if (it != uniformLocations.end())
			return (*it).second;
		return -1;
	};

}
