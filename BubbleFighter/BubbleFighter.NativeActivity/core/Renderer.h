#pragma once

#include <memory>
#include <functional>

#include <vector>
#include <list>
#include <unordered_map>


#include <GLES2/gl2.h>


#include "Texture2D.h"
#include "Shader.h"
#include "Program.h"
#include "Material.h"



namespace Graphic
{

	inline bool glIsError(GLenum errNum)
	{
		return errNum != GL_NO_ERROR;
	};


	class Renderer
	{

	private:

		virtual void logErrors(GLenum errNum) = 0;


	public:
		virtual PTexture2D loadTexture(const std::string& _textureName)
		{
			Texture2D* newTex = nullptr;
			GLuint texId = -1;

			glGenTextures(1, &texId);

			void glTexParameteri(GLenum target, GLenum pname, GLint param);

			void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data);

			GLenum glGetError(void);




			//newTex = new Texture2D(_textureName,texId);
			//newTex->setParameter
		};


		virtual void unloadTexture(Texture2D* texture) = 0;

		virtual PShader loadShader() = 0;
		virtual void unloadShader(Shader* shader) = 0;

		virtual PProgram loadProgram() = 0;
		virtual void unloadProgram(Program* program) = 0;

		virtual PMaterial loadMaterial() = 0;
		virtual void unloadMaterial(Material* material) = 0;
	};





	class RenderChunk2D
	{
	private:
		Material* material;



	};

}
