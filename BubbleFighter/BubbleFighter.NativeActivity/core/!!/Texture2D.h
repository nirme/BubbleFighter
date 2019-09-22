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
		int width;
		int height;

		//std::vector<std::pair< GLenum, GLint>> params;

	public:

		Texture2D::Texture2D(std::string _name, GLuint _id, GLint _format, int _width, int _height)
			: id(_id), name(_name), format(_format), width(_width), height(_height)
		{};

		Texture2D::Texture2D(Texture2D&& t)
			: id(t.id), format(t.format), width(t.width), height(t.height)
		{
			name.swap(t.name);
			//params.swap(t.params);

			t.id = 0;
			t.format = 0;
			t.width = 0;
			t.height = 0;
		};

		//		bool setParameter(GLenum pName, GLint pValue);
//		GLint getParameterByName(GLenum pName);
//		int getParametersNumber();
//		std::pair< GLenum, GLint> getParameterByIndex(unsigned int i);

		inline GLuint getId() const
		{
			return id;
		};

		const std::string& getName() const;

		inline GLint getFormat() const
		{
			return format;
		};

		inline int getWidth() const
		{
			return width;
		};

		inline int getHeight() const
		{
			return height;
		};
	};


	typedef std::shared_ptr<Texture2D> PTexture2D;

}