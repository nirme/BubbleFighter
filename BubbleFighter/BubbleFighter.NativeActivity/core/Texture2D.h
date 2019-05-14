#pragma once

#include <memory>
#include <vector>
#include <GLES2/gl2.h>



namespace Graphic
{

	class Texture2D
	{
	private:
		std::string name;
		GLuint id;

		GLint format;
		GLsizei width;
		GLsizei height;

		std::vector<std::pair< GLenum, GLint>> params;

	public:

		Texture2D(std::string _name, GLuint _id);
		Texture2D(Texture2D&& t);

		bool setParameter(GLenum pName, GLint pValue);
		GLint getParameterByName(GLenum pName);
		int getParametersNumber();
		std::pair< GLenum, GLint> getParameterByIndex(unsigned int i);

		inline GLuint getId() const
		{
			return id;
		};

		const std::string& getName() const;

		inline int getWidth() const
		{
			return (int)width;
		};

		inline int getHeight() const
		{
			return (int)height;
		};
	};


	typedef std::shared_ptr<Texture2D> PTexture2D;

}