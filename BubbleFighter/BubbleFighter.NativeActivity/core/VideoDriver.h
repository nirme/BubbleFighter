#pragma once

#include <cassert>

#include <memory>
#include <functional>

#include <string>

#include <vector>
#include <list>
#include <unordered_map>

#include <boost/variant.hpp>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "Texture2D.h"
#include "Shader.h"
#include "Program.h"
//#include "Material.h"
#include "Billboard.h"

#include "vec.h"



namespace Graphic
{
	class Position2D;


	struct RenderChunk2D
	{
		PBillboard billboard;

		const Position2D* position;

		unsigned char depth;

		/*
		inline void generateTransformationMatrix(Mx3d& _mx)
		{
			mxScaleRotateTranslate(Mx3d& _m, float _scaleX, float _scaleY, float _rotationAngle, float _translateX, float _translateY);
			billboard->
			position->getScale().x
			position->getScale().y
		};
		*/
	};

	class Material;


	class VideoDriver
	{
		//friend class Material;
		friend void Material::apply(VideoDriver*) const;

	private:


		inline static unsigned int maxBillboardsPerDrawCall = 50;


		std::vector<std::pair<GLuint, std::string>> defaultAttribLocations; // should always be sorted

		std::set<GLuint> allocatedTextureIds;
		std::set<GLuint> allocatedShaderIds;
		std::set<GLuint> allocatedProgramIds;



		// billboard drawing information
		bool billboardBuffersAlocated;
		GLuint index2DBufferId, vertex2DBufferId;
		std::vector<float> vertex2DBufferData;
		GLenum billboardDrawMode;
		GLenum bilboardIndexType;




		void setupBillboardsBuffers();
		void deleteBillboardsBuffers();



		EGLDisplay eglDisplay; // EGL_NO_DISPLAY
		EGLConfig eglConfig;
		EGLContext eglContext;
		EGLSurface eglSurface;
		int eglSurfaceWidth, eglSurfaceHeight;


		inline static const EGLint minimalDisplayAttributesConfig[] = {
			EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,		//gles2
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,		//full window
			EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,	//RGB buffers only
			EGL_NATIVE_RENDERABLE, EGL_TRUE,		// preferably native renderable
			EGL_BLUE_SIZE, 8,						//8bit blue channel
			EGL_GREEN_SIZE, 8,						//8bit green channel
			EGL_RED_SIZE, 8,						//8bit red channel
			EGL_NONE
		};




		EGLBoolean getMatchingDisplayConfig(EGLDisplay display, EGLConfig *config)
		{
			EGLBoolean ret;
			EGLint numConfigs;

			// get amount of configs fitting
			ret = eglChooseConfig(display, &(minimalDisplayAttributesConfig[0]), NULL, 1, &numConfigs);
			if (ret != EGL_TRUE) return ret;
			if (numConfigs == 0) return EGL_FALSE;

			// get actual config list
			std::vector<EGLConfig> configs(numConfigs);
			ret = eglChooseConfig(display, &(minimalDisplayAttributesConfig[0]), &(configs[0]), configs.size(), &numConfigs);
			if (ret != EGL_TRUE) return ret;

			bool matchFlag;
			EGLint value;
			int i;

			for (int c = 0; c < numConfigs; ++c)
			{
				matchFlag = true;
				i = 0;
				while (minimalDisplayAttributesConfig[i] != EGL_NONE && matchFlag)
				{
					eglGetConfigAttrib(display, configs[c], minimalDisplayAttributesConfig[i], &value);
					if (ret != EGL_TRUE) return ret;

					switch (minimalDisplayAttributesConfig[i])
					{
					//bitmasked values, can have more that just what requested
					case EGL_CONFORMANT:
					case EGL_RENDERABLE_TYPE:
					case EGL_SURFACE_TYPE:
						matchFlag = (value & minimalDisplayAttributesConfig[i + 1]) == minimalDisplayAttributesConfig[i + 1];
						break;

					default:
						matchFlag = value == minimalDisplayAttributesConfig[i + 1];
					}

					i += 2;
				}

				if (matchFlag)
				{
					*config = configs[c];
					return EGL_TRUE;
				}
			}

			return EGL_FALSE;
		};



		void initializeDisplay(ANativeWindow* aNativeWindow)
		{
			try
			{
				EGLint error;
				// get egl display
				eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
				if ((error = eglGetError()) != EGL_SUCCESS)	throw EGLException(error);

				// init display and get api version
				EGLint eglImplVersionMajor, eglImplVersionMinor;
				EGLBoolean ret = eglInitialize(eglDisplay, &eglImplVersionMajor, &eglImplVersionMinor);
				if ((error = eglGetError()) != EGL_SUCCESS)	throw EGLException(error);

				// get most matching config
				ret = getMatchingDisplayConfig(eglDisplay, &eglConfig);
				if ((error = eglGetError()) != EGL_SUCCESS)	throw EGLException(error);


				EGLint visualId;
				ret = eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &visualId);
				if ((error = eglGetError()) != EGL_SUCCESS)	throw EGLException(error);


				int32_t nativeRet = ANativeWindow_setBuffersGeometry(aNativeWindow, 0, 0, visualId);
				if (nativeRet != 0) throw AndroidException(nativeRet, "ANativeWindow_setBuffersGeometry");

				eglSurfaceWidth = ANativeWindow_getWidth(aNativeWindow);
				if ((error = eglGetError()) != EGL_SUCCESS)	throw EGLException(error);

				eglSurfaceHeight = ANativeWindow_getHeight(aNativeWindow);
				if ((error = eglGetError()) != EGL_SUCCESS)	throw EGLException(error);


				//crate context for rendering
				EGLint contextAttribs[] =
				{
					EGL_CONTEXT_CLIENT_VERSION, 2, // Specifies OpenGL ES 2.0.
					EGL_NONE
				};
				eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttribs);
				if ((error = eglGetError()) != EGL_SUCCESS)	throw EGLException(error);


				// create render surface
				eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, aNativeWindow, NULL);
				if ((error = eglGetError()) != EGL_SUCCESS)	throw EGLException(error);


				ret = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
				if ((error = eglGetError()) != EGL_SUCCESS)	throw EGLException(error);


				GLenum glError;
				glViewport(0, 0, eglSurfaceWidth, eglSurfaceHeight);
				if ((glError = glGetError()) != GL_NO_ERROR) throw GlesException(glError);

				glScissor(0, 0, eglSurfaceWidth, eglSurfaceHeight);
				if ((glError = glGetError()) != GL_NO_ERROR) throw GlesException(glError);

			}
			catch (...)
			{
				if (eglContext != EGL_NO_CONTEXT)
					eglDestroyContext(eglDisplay, eglContext);

				if (eglSurface != EGL_NO_SURFACE)
					eglDestroySurface(eglDisplay, eglSurface);

				eglConfig = NULL;
				eglSurface = EGL_NO_SURFACE;
				eglContext = EGL_NO_CONTEXT;
				eglDisplay = EGL_NO_DISPLAY;

				throw;
			}
		};


		void reaquireContext();


		void destroyDisplay()
		{
		};




		virtual void logErrors(GLenum errNum) = 0;



		inline void _setProgram(GLuint programId)
		{
			glUseProgram(programId);
		};


		inline void _setTexture2D(unsigned short textureUnitIndex, GLuint textureId)
		{
			glActiveTexture(GL_TEXTURE0 + textureUnitIndex);
			glBindTexture(GL_TEXTURE_2D, textureId);
		};

		void applyMaterial(const Material* material)
		{
			material->apply(this);
		};


	public:

		void render(const RenderChunk2D* chunk)
		{
			// bind index data
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index2DBufferId);

			// bind and fillout vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertex2DBufferId);

			int vertexData = chunk->billboard->getVertexSize();
			int stride = vertexData - 2 * sizeof(float);

			chunk->billboard->fillCombinedVertexData(&(vertex2DBufferData[0]));
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData, vertex2DBufferData.data());

			// apply material (program and texture)
			chunk->billboard->getMaterialPtr()->apply(this);


			for (unsigned short i = 0; i < defaultAttribLocations.size(); ++i)
			{
				// set vertex attributes
				glEnableVertexAttribArray(defaultAttribLocations[i].first);
				glVertexAttribPointer(defaultAttribLocations[i].first, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid *) (i * 2 * sizeof(float)));
			}
			
			
			glDrawElements(billboardDrawMode, 2 * 3, bilboardIndexType, NULL);


			for (unsigned short i = 0; i < defaultAttribLocations.size(); ++i)
			{
				// unset vertex attributes
				glDisableVertexAttribArray(defaultAttribLocations[i].first);
			}

		};




		void setAttribLocation(unsigned int index, std::string name)
		{
			for (int i = 0; i < defaultAttribLocations.size(); ++i)
			{
				if (defaultAttribLocations[i].first == (GLuint)index)
				{
					defaultAttribLocations[i].second = name;
					return;
				}
			}

			defaultAttribLocations.push_back(std::make_pair((GLuint)index, name));
		};



		virtual GLuint loadTexture2D(
			GLsizei _width, 
			GLsizei _height, 
			GLenum _format, 
			GLenum _dataType, 
			const std::vector<std::pair<GLenum,boost::variant<GLint, GLfloat>>>& _parameters, 
			const std::vector<unsigned char>& _data,
			bool _isCompressed = false);
		virtual void unloadTexture(GLuint textureId);

		virtual GLuint loadShader(GLenum shaderType, const std::string& shaderSource);
		virtual void unloadShader(GLuint shaderId);

		virtual GLuint loadProgram(std::vector<GLuint> _shaders);
		virtual void unloadProgram(GLuint programId);








	};



	class ApplicationException : public std::exception
	{
	protected:
		std::string errorMsg;

	public:
		ApplicationException(std::string _errorMsg)
			: errorMsg(_errorMsg)
		{};

		virtual const char* what() const noexcept
		{
			return errorMsg.c_str();
		};
	};

	class AndroidException : public ApplicationException
	{
	protected:
		int32_t error;

	public:
		AndroidException(int32_t _error, std::string _functionName)
			: ApplicationException("Android function '" + _functionName + "' failed with code: " ), error(_error)
		{
			//errorMsg += std::to_string(_error);

			char num[12];
			sprintf(num, "%#08X", error);
			errorMsg += num;
		};
	};



	class EGLException : public std::exception
	{
	protected:
		EGLint error;
		const char* errorMsg;

	public:
		EGLException(EGLint _error) {};
		virtual const char* what() const noexcept {};
	};



	class GlesException : public std::exception
	{
	protected:
		GLenum error;
		const char* errorMsg;

	public:
		GlesException(GLenum _error);
		virtual const char* what() const noexcept;
	};

	class GlesExceptionWithInfo : public GlesException
	{
	protected:
		std::string infoLog;

	public:
		GlesExceptionWithInfo(std::string& _infoLog);
		const std::string& getInfoLog() const;
	};

	class GlesCompileError : public GlesExceptionWithInfo
	{
	public:
		GlesCompileError(std::string& _infoLog);
	};

	class GlesLinkingError : public GlesExceptionWithInfo
	{
	public:
		GlesLinkingError(std::string& _infoLog);
	};

}
