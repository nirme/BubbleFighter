#include "RenderSystem.h"

namespace core
{

	void RenderSystem::createBatchingBuffer(unsigned int _bufferSize)
	{
		assert(!initialized && "Cannot create batching buffer, renderer not initialized");

		try
		{
			GLuint bufferId[2] = { 0,0 };

			GL_ERROR_CHECK(glGenBuffers(2, bufferId));

			batchingIndexBufferId = bufferId[0];
			batchingVertexBufferId = bufferId[1];

			batchingBufferArray.resize(_bufferSize);
			freeBufferPosition = 0;
			batchingBufferArray.shrink_to_fit();

			unsigned long indexCount = (_bufferSize / (sizeof(float) * 4 * 4)) * 6;

			// generate indices for use with batched sprites
			{
				unsigned char _indices[6] = { 0, 2, 3, 0, 3, 1 };
				for (unsigned long i = 0, j = 0; i < indexCount; ++i, j = ++j % 6)
					batchingBufferArray[i] = _indices[j];
			}

			// fill index buffer
			GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchingIndexBufferId));
			GL_ERROR_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount, batchingBufferArray.data(), GL_STATIC_DRAW));


			GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, batchingVertexBufferId));
			GL_ERROR_CHECK(glBufferData(GL_ARRAY_BUFFER, _bufferSize, 0, GL_DYNAMIC_DRAW));
		}
		catch (const std::exception &e)
		{
			if (batchingIndexBufferId)
			{
				glDeleteBuffers(1, &batchingIndexBufferId);
				glGetError();
				batchingIndexBufferId = 0;
			}

			if (batchingVertexBufferId)
			{
				glDeleteBuffers(1, &batchingVertexBufferId);
				glGetError();
				batchingVertexBufferId = 0;
			}

			throw;
		}
	};


	void RenderSystem::deleteBatchingBuffer()
	{
		assert(!initialized && "Cannot delete batching buffer, renderer not initialized");

		if (batchingIndexBufferId)
		{
			glDeleteBuffers(1, &batchingIndexBufferId);
			glGetError();
			batchingIndexBufferId = 0;
		}

		if (batchingVertexBufferId)
		{
			glDeleteBuffers(1, &batchingVertexBufferId);
			glGetError();
			batchingVertexBufferId = 0;
		}

		batchingBufferArray.resize(0);
		freeBufferPosition = 0;
		batchingBufferArray.shrink_to_fit();
	};




	RenderSystem::GraphicState::GraphicState() :
		hwBlenging(false),
		blenginSfactor(GL_ONE),
		blenginDfactor(GL_ZERO),
		hwFaceCulling(false),
		cullingMode(GL_BACK),
		hwDepthTest(false),
		depthTestFunction(GL_LESS),
		depthTestNearVal(0.0f),
		depthTestFarVal(1.0f),
		hwDither(true)
		//hwPolygonOffsetFill(false),
		//polyOffsetFillFactor(0.0f),
		//polyOffsetFillUnits(0.0f)
	{};



	void RenderSystem::useMultisampling(bool _multisampling)
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




	bool RenderSystem::initialize()
	{
		if (initialized)
		{
			Logger::getSingleton().write("Render System already initialized");
			return true;
		}


		try
		{
			EGLDisplay tmpDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

			EGLint eglVerMajor, eglVerMinor;
			EGL_ERROR_CHECK(eglInitialize(tmpDisplay, &eglVerMajor, &eglVerMinor));

			display = tmpDisplay;


			//check egl ver if needed

			EGLConfig eglConfig((EGLConfig)0);

			if (!cashedWindowConfigID)
			{
				EGLint numConfigs;
				EGL_ERROR_CHECK(eglChooseConfig(display, windowSurfaceAttribs, nullptr, 0, &numConfigs));

				std::vector<EGLConfig> configs(numConfigs);
				EGL_ERROR_CHECK(eglChooseConfig(display, windowSurfaceAttribs, configs.data(), numConfigs, &numConfigs));

				AttribList bestConfig(wndSurfAttrPreferHighest.size(), 0);


				for (long i = 0; i < numConfigs; ++i)
				{
					const EGLint *attributePair = windowSurfaceAttribs;
					EGLint attributeValue;

					bool configMatching = true;
					while (attributePair[0] != EGL_NONE)
					{
						EGL_ERROR_CHECK(eglGetConfigAttrib(display, configs[i], attributePair[0], &attributeValue));

						if (attributePair[1] != attributeValue)
						{
							configMatching = false;
							break;
						}

						attributePair += 2;
					}

					if (configMatching)
					{
						AttribList attributesList;
						for (long j = 0; j < wndSurfAttrPreferHighest.size(); ++j)
							EGL_ERROR_CHECK(eglGetConfigAttrib(display, configs[i], wndSurfAttrPreferHighest[j], &(attributesList[j])));

						if (!eglConfig ||
							eglConfig && bestConfig < attributesList)
						{
							bestConfig.swap(attributesList);
							eglConfig = configs[i];
						}
					}
				}

				if (!eglConfig)
					throw std::runtime_error("No frame buffer configuration matching selected attributes");

				// cache config id for quick resume
				EGL_ERROR_CHECK(eglGetConfigAttrib(display, eglConfig, EGL_CONFIG_ID, &cashedWindowConfigID));
			}
			else
			{
				EGLint windowAttribConfigID[] = {
					EGL_CONFIG_ID, cashedWindowConfigID,
					EGL_NONE
				};

				EGL_ERROR_CHECK(eglChooseConfig(display, windowAttribConfigID, &eglConfig, 1, nullptr));
			}


			EGLint nativeVisualFormat;
			EGL_ERROR_CHECK(eglGetConfigAttrib(display, eglConfig, EGL_NATIVE_VISUAL_ID, &nativeVisualFormat));


			// native window
			{
				ANativeWindow* window(nullptr);
				// leave w/h as 0 to reset to window base
				int32_t res = ANativeWindow_setBuffersGeometry(window, 0, 0, nativeVisualFormat);
				if (res < 0)
				{
					std::string message("ANativeWindow_setBuffersGeometry failed with return value ");
					message += res;
					throw std::runtime_error(message);
				}

				androidApp->window = window;
			}


			// egl surface
			{
				EGLSurface tmpSurface;
				EGL_ERROR_CHECK(tmpSurface = eglCreateWindowSurface(display, eglConfig, androidApp->window, NULL));
				surface = tmpSurface;
			}

			// egl context
			{
				EGLContext tmpContext;
				EGL_ERROR_CHECK(tmpContext = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, glVersion));
				context = tmpContext;
			}

			EGL_ERROR_CHECK(eglMakeCurrent(display, surface, surface, context));


			EGLint width(0), height(0);
			EGL_ERROR_CHECK(eglQuerySurface(display, surface, EGL_WIDTH, &width));
			EGL_ERROR_CHECK(eglQuerySurface(display, surface, EGL_HEIGHT, &height));

			screenWidth = width;
			screenHeight = height;

			// Initialize GL state.
			setGLState(defaultState, false);

			// generate batch buffer
			createBatchingBuffer();
		}
		catch (const std::exception &e)
		{
			// egl/gles setup failed
			Logger::getSingleton().write(e.what(), LL_ERROR);

			screenWidth = 0;
			screenHeight = 0;

			if (context)
			{
				eglDestroyContext(display, context);
				eglGetError();
				context = (EGLContext)0;
			}

			if (surface)
			{
				eglDestroySurface(display, surface);
				eglGetError();
				surface = (EGLSurface)0;
			}

			androidApp->window = nullptr;

			if (display)
			{
				eglTerminate(display);
				eglGetError();
				display = (EGLDisplay)0;
			}

			// quit the function 
			return false;
		}

		initialized = true;
		return true;
	};



	void RenderSystem::uninitialize()
	{
		if (!initialized)
		{
			Logger::getSingleton().write("Render System already uninitialized");
			return;
		}

		initialized = false;

		deleteBatchingBuffer();

		screenWidth = 0;
		screenHeight = 0;
		androidApp->window = nullptr;

		if (context)
		{
			eglDestroyContext(display, context);
			context = (EGLContext)0;
			notifyOnContextLost();
		}

		if (surface)
		{
			eglDestroySurface(display, surface);
			surface = (EGLSurface)0;
		}

		if (display)
		{
			eglTerminate(display);
			display = (EGLDisplay)0;
		}

		// cleanup any errors that might have occured
		eglGetError();
	};



	void RenderSystem::reinitialize()
	{
		if (initialized)
			uninitialize();

		initialize();
		notifyOnContextReaquired();
	};






	void RenderSystem::setGLState(const GraphicState &_state, bool _optimizeStateChange)
	{
		if (!_optimizeStateChange || _state.hwBlenging != currentState.hwBlenging)
		{
			if (_state.hwBlenging)
			{
				GL_ERROR_CHECK(glBlendFunc(_state.blenginSfactor, _state.blenginDfactor));
				GL_ERROR_CHECK(glEnable(GL_BLEND));
			}
			else
			{
				GL_ERROR_CHECK(glDisable(GL_BLEND));
			}
		}

		if (!_optimizeStateChange || _state.hwFaceCulling != currentState.hwFaceCulling)
		{
			if (_state.hwFaceCulling)
			{
				GL_ERROR_CHECK(glCullFace(_state.cullingMode));
				GL_ERROR_CHECK(glEnable(GL_CULL_FACE));
			}
			else
			{
				GL_ERROR_CHECK(glDisable(GL_CULL_FACE));
			}
		}

		if (!_optimizeStateChange || _state.hwDepthTest != currentState.hwDepthTest)
		{
			if (_state.hwDepthTest)
			{
				GL_ERROR_CHECK(glDepthFunc(_state.depthTestFunction));
				GL_ERROR_CHECK(glDepthRangef(_state.depthTestNearVal, _state.depthTestFarVal));
				GL_ERROR_CHECK(glEnable(GL_DEPTH_TEST));
			}
			else
			{
				GL_ERROR_CHECK(glDisable(GL_DEPTH_TEST));
			}
		}

		if (!_optimizeStateChange || _state.hwDither != currentState.hwDither)
		{
			if (_state.hwDither)
			{
				GL_ERROR_CHECK(glEnable(GL_DITHER));
			}
			else
			{
				GL_ERROR_CHECK(glDisable(GL_DITHER));
			}
		}

		currentState = _state;
	};



}
