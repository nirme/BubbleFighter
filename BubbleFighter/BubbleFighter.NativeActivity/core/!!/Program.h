#pragma once

#include <memory>
#include <list>
#include <unordered_map>

#include "Shader.h"



namespace Graphic
{


	class Program
	{
		std::string name;
		GLuint id;

		std::list<std::pair<GLenum, PShader>> attachedShaders;
		std::unordered_map<std::string, GLuint> uniformLocations;

	public:

		Program(std::string _name, GLuint _id);
		Program(Program&& p);

		inline GLuint getId() const
		{
			return id;
		};

		std::string getName() const;

		void setAttachedShader(PShader shader);
		void setUniformId(const std::string& uniformName, GLint uniformId);
		GLint getUniformId(const std::string& uniformName);
	};


	typedef std::shared_ptr<Program> PProgram;
}
