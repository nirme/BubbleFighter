#pragma once

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <stdint.h>
#include <list>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "SingletonTemplate.h"
#include "Exceptions.h"

#include "Logger.h"
#include "Color.h"

#include "_2d/Renderable.h"


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

		EGLint cashedWindowConfigID;

		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;

		unsigned short screenWidth, screenHeight;

		bool hwMultisampling;

		bool useDepth;
		bool useStencil;

		ColorF bgColor;
		GLclampf baseDepth;
		GLint baseStancil;


		std::vector<uchar> batchingBufferArray;
		unsigned long freeBufferPosition;
		GLuint batchingVertexBufferId;
		GLuint batchingIndexBufferId;



		void createBatchingBuffer(unsigned int _bufferSize = 8192); //default to 8k - around 250 sprites ber batch
		void deleteBatchingBuffer();



		struct GraphicState
		{
			bool hwBlenging; //GL_BLEND
			GLenum blenginSfactor;
			GLenum blenginDfactor;

			bool hwFaceCulling; //GL_CULL_FACE
			GLenum cullingMode; //GL_BACK, GL_FRONT, GL_FRONT_AND_BACK

			bool hwDepthTest; //GL_DEPTH_TEST
			GLenum depthTestFunction; //GL_LESS, GL_NEVER, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS
			GLclampf depthTestNearVal; //0.0f
			GLclampf depthTestFarVal; //1.0f

			bool hwDither; //GL_DITHER


			//GL_SAMPLE_ALPHA_TO_COVERAGE
			//GL_SAMPLE_COVERAGE
			//GL_SCISSOR_TEST
			//GL_STENCIL_TEST

			GraphicState();
		};

		GraphicState defaultState;
		GraphicState currentState;


		//
		typedef std::vector<EGLint> AttribList;
		AttribList wndSurfAttrPreferHighest = {
			EGL_DEPTH_SIZE,
		};



		
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








		static constexpr EGLint windowSurfaceAttribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_ALPHA_SIZE, 0, //no alpha for render end surface
			EGL_BUFFER_SIZE, 24, //RGB8 = 24
			EGL_CONFORMANT, EGL_OPENGL_ES2_BIT, //GLES2
			EGL_NONE
		};


		static constexpr EGLint glVersion[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2, // GLES 2 at least
			EGL_NONE
		};









	public:


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



		void useMultisampling(bool _multisampling);


		bool initialize();
		void uninitialize();
		void reinitialize();




		void notifyOnContextLost()
		{};

		void notifyOnContextReaquired()
		{};


		void setGLState(const GraphicState &_state, bool _optimizeStateChange = true);



		void begin()
		{
			GLbitfield clearMask = GL_COLOR_BUFFER_BIT;
			GL_ERROR_CHECK(glClearColor(bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha));

			if (useDepth)
			{
				clearMask |= GL_DEPTH_BUFFER_BIT;
				GL_ERROR_CHECK(glClearDepthf(baseDepth));
			}

			if (useStencil)
			{
				clearMask |= GL_STENCIL_BUFFER_BIT;
				GL_ERROR_CHECK(glClearStencil(baseStancil));
			}

			GL_ERROR_CHECK(glClear(clearMask));
		};


		void end()
		{
			EGL_ERROR_CHECK(eglSwapBuffers(display, surface));
		};


		void render(const _2d::Renderable &_renderable)
		{
			//_renderable.getTransform();
		};




		static const std::array<Vector2,4> 






		/*
		enum GRAPHIC_CAPABILITY
		{
			TEXTURE_BGRA_8888 = 0x0001, //EXT_texture_format_BGRA8888
		};

		*/


	};

}