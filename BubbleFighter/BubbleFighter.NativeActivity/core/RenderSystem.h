#pragma once

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <stdint.h>
#include <list>
#include <utility>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "SingletonTemplate.h"
#include "Exceptions.h"

#include "RenderStateCashe.h"

#include "Logger.h"
#include "Color.h"

#include "_2d/Renderable.h"
#include "GraphicBuffer.h"
#include "_2d/RenderQueue.h"
#include "_2d/MaterialManager.h"


/*
   0---1
1  | \ |  2
   2---3

   0,2,3
   0,3,1

*/


typedef std::list<std::pair<GLenum, GLint>> ParamList;


namespace core 
{

	class RenderSystem
	{
	protected:

		typedef unsigned char uchar;

		bool initialized;

		android_app* androidApp;

		static constexpr EGLint windowSurfaceAttribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_ALPHA_SIZE, 0, //no alpha for render end surface
			EGL_BUFFER_SIZE, 24, //RGB8 = 24
			EGL_DEPTH_SIZE, 0, //no depth buffer needed for 2D
			EGL_CONFORMANT, EGL_OPENGL_ES2_BIT, //GLES2
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //GLES2
			EGL_NONE
		};

		typedef std::set<EGLint> AttribList;
		AttribList wndSurfAttrPreferHighest;

		static constexpr EGLint glContextAttribs[] = {
			//EGL_CONTEXT_CLIENT_VERSION, 2, //GLES2 - older version
			EGL_CONTEXT_MAJOR_VERSION, 2, //GLES2
			EGL_NONE
		};

		EGLint cashedWindowConfigID;

		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;


		unsigned short screenWidth, screenHeight;

		bool hwMultisampling;

		ColorF bgColor;


		_2d::MaterialManager materialManager;

		GraphicBuffer batchingVertexBuffer;
		unsigned int batchedSprites;

		GraphicBuffer singleVertexBuffer;
		GraphicBuffer indexBuffer;


		RenderStateCashe state;



		void createBuffers(unsigned int _bufferSize = 8192); //default to 8k - around 250 sprites ber batch
		void deleteBuffers();



		/*
		RenderSystem(android_app* _androidApp) : 
			androidApp(_androidApp),
			initialized(false), 
			cashedWindowConfigID(0), 
			display((EGLDisplay)0),
			surface((EGLSurface)0),
			context((EGLContext)0),
			screenWidth(0), 
			screenHeight(0),
			hwMultisampling(false)
		{
			assert(androidApp && "Application state object cannot be nullptr");

		};
		*/



	public:

		/*
		RenderSystem() : 
			initialized(false), 
			androidApp(nullptr), 
			cashedWindowConfigID(0), 
			display((EGLDisplay)0), 
			surface((EGLSurface)0),
			context((EGLContext)0),
			screenWidth(0),
			screenHeight(0),
			hwMultisampling(false)
		{};
		*/


		// enable multisampling
		void enableMultisampling(bool _multisampling)
		{
			if (_multisampling)
			{
				wndSurfAttrPreferHighest.insert(EGL_SAMPLE_BUFFERS);
				wndSurfAttrPreferHighest.insert(EGL_SAMPLES);
			}
			else
			{
				wndSurfAttrPreferHighest.erase(EGL_SAMPLE_BUFFERS);
				wndSurfAttrPreferHighest.erase(EGL_SAMPLES);
			}

			if (hwMultisampling != _multisampling)
			{
				hwMultisampling = _multisampling;
				reinitialize();
			}
		};


		bool initialize();
		void uninitialize();
		void reinitialize();


		bool hasContext() const
		{
			return context != nullptr;
		};

		RenderStateCashe &getStateCashe()
		{
			return state;
		};


		void notifyOnContextLost()
		{};

		void notifyOnContextAquired()
		{};


		GraphicBuffer &getBatchingVertexBuffer()
		{
			return batchingVertexBuffer;
		};

		GraphicBuffer &getIndexVertexBuffer()
		{
			return indexBuffer;
		};



		void beforeRendering()
		{
			GLbitfield clearMask = GL_COLOR_BUFFER_BIT;
			GL_ERROR_CHECK(glClearColor(bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha));

			GL_ERROR_CHECK(glClear(clearMask));
		};


		void afterRendering()
		{
			EGL_ERROR_CHECK(eglSwapBuffers(display, surface));
		};


		void applyVertexAttribs(const ShadingProgram::VertexAttribList &_attribList);

		void render(const _2d::Renderable *_renderable, const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough);
		void singleRender(const _2d::Renderable *_renderable, const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough);
		void bufferedRender(const _2d::Renderable *_renderable, const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough);
		void flushRenderOp(const ShadingProgram *_program, const ShadingParamsPassthru *_paramsPassthrough);


	};

}