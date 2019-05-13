#pragma once

#include <memory>


namespace Graphic
{


	class Shader
	{
	private:
		std::string name;
		GLuint id;

		GLenum type; //GL_VERTEX_SHADER or GL_FRAGMENT_SHADER

	public:

		Shader(std::string _name, GLuint _id, GLenum _type);
		Shader(Shader&& s);

		inline GLuint getId() const
		{
			return id;
		};

		std::string getName() const;
		GLenum getType() const;
	};

	typedef std::shared_ptr<Shader> PShader;
}
