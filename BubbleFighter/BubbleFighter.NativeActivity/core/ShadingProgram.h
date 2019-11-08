#pragma once

#include <memory>
#include <array>
#include <map>
#include <unordered_map>
#include <GLES2/gl2.h>

#include "Exceptions.h"
#include "Logger.h"

#include "Resource.h"
#include "Shader.h"
#include "ShaderManager.h"


namespace core
{

	struct Attrib
	{
		GLint id;
		GLint size;
		GLenum type;

		Attrib(GLint _id = -1, GLint _size = 0, GLenum _type = 0) :
			id(_id), size(_size), type(_type)
		{};
	};

	struct Uniform
	{
		std::string name;
		GLint id;
		GLint size;
		GLenum type;

		Uniform(std::string _name, GLint _id, GLint _size, GLenum _type) :
			name(_name), id(_id), size(_size), type(_type)
		{};
	};



	enum VERTEX_ATTRIB
	{
		VA_POSITION = 0x00,
		VA_TEXTURE1 = 0x01,
		VA_TEXTURE2 = 0x02,

		VA_ENUM_COUNT = 0X03, // do not use
	};


	class ShadingProgram : public Resource
	{
	protected:

		static constexpr std::array<const char*, VA_ENUM_COUNT> vertexAttribNames{{ "position", "tex1", "tex2"}};

		struct ShaderDef
		{
			std::string name;
			ShaderPtr shader;
		};

		ShaderDef verterShader;
		ShaderDef fragmentShader;

		GLuint id;

		std::array<Attrib, VA_ENUM_COUNT> vertexAttribs;

		typedef std::map<std::string, Uniform> UniformNamedMap;
		UniformNamedMap uniformMap;



	public:

		ShadingProgram(std::string _name, ResourceHandle _handle, std::string _group, ResourceManager *_manager = nullptr);
		~ShadingProgram();

		void loadImp();
		void unloadImp();

		unsigned int sizeCalcImpl();

		inline const Attrib& getAttrib(VERTEX_ATTRIB _attrib)
		{
			return vertexAttribs[_attrib];
		};

		inline GLint getId()
		{
			return id;
		};

		void setShader(std::string _shaderName);
	};

	typedef std::shared_ptr<ShadingProgram> ShadingProgramPtr;
}
