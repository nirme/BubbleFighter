#include "RenderSystem.h"

namespace core
{

	void RenderSystem::createBatchingBuffer(unsigned int _bufferSize)
	{
		assert(!initialized && "Cannot create batching buffer, renderer not initialized");

		batchingVertexBufferId = GraphicBuffer(GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER, GL_FLOAT);

		// round down to matching multiple of float size fitting in _bufferSize
		unsigned int vertexBuffSize = ((int)(_bufferSize / sizeof(float))) * sizeof(float);
		batchingVertexBufferId.resize(vertexBuffSize);
		batchingVertexBufferId.load();

		unsigned int indexBuffSize = ((vertexBuffSize / 16 / sizeof(float)) + 1); //(amount of smallest sprites + 1)

		batchingIndexBufferId = GraphicBuffer(GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_SHORT);
		batchingIndexBufferId.resize(indexBuffSize);


		batchingIndexBufferId.load();

		// generate indices for use with batched sprites
		unsigned short _indices[6] = { 0, 2, 3, 0, 3, 1 };
			
		for (unsigned int i = 0; i < indexBuffSize; ++i)
		{
			batchingIndexBufferId.write(_indices, 6);

			_indices[0] += 4;
			_indices[1] += 4;
			_indices[2] += 4;
			_indices[3] += 4;
			_indices[4] += 4;
			_indices[5] += 4;
		}

		batchingIndexBufferId.uploadData();
	};


	void RenderSystem::deleteBatchingBuffer()
	{
		assert(!initialized && "Cannot delete batching buffer, renderer not initialized");

		batchingVertexBufferId.unload();
		batchingVertexBufferId.resize(0);

		batchingIndexBufferId.unload();
		batchingIndexBufferId.resize(0);
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


				for (int i = 0; i < numConfigs; ++i)
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
						for (int j = 0; j < wndSurfAttrPreferHighest.size(); ++j)
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
				EGL_ERROR_CHECK(tmpContext = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, glContextAttribs));
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
		while (eglGetError() != EGL_SUCCESS);
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









	void RenderSystem::useProgram(const ShadingProgramPtr _program)
	{
		for (unsigned int i = 0; i < 8; ++i)
		{
			GL_ERROR_CHECK(glDisableVertexAttribArray((GLuint)i));
		}

		GL_ERROR_CHECK(glUseProgram(_program->getId()));
		const ShadingProgram::VertexAttribList &attribs = _program->getAttribList();

		for (unsigned int i = 0; i < attribs.size(); ++i)
		{
			GL_ERROR_CHECK(glEnableVertexAttribArray(attribs[i].id));
			GL_ERROR_CHECK(glVertexAttribPointer(attribs[i].id, attribs[i].size, attribs[i].type, GL_FALSE, attribs[i].stride, (GLvoid*)attribs[i].offsetInBytes));
		}

		usedProgram = _program;
	};

	void RenderSystem::useTexture(unsigned int _index, const TexturePtr _texture)
	{
		assert(_index < 8 && "Cannot access texture units above 8");

		if (usedTextures[_index] != _texture)
		{
			GL_ERROR_CHECK(glActiveTexture(GL_TEXTURE0 + _index));
			GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, _texture->getId()));

			usedTextures[_index] = _texture;
		}
	};


	void RenderSystem::flushBufferedRenderables()
	{
		if (batchingVertexBufferId.position())
		{
			batchingVertexBufferId.uploadData();
			unsigned int count = (batchingVertexBufferId.position() * batchingVertexBufferId.elementSize()) / (usedProgram->getVertexSize() * 3);

			GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, batchingVertexBufferId.getId()));
			GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchingIndexBufferId.getId()));

			GL_ERROR_CHECK(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr));

			batchingVertexBufferId.rewind();
		}
	};


	void RenderSystem::render(const _2d::Renderable *_renderable)
	{
		using namespace _2d;

		if (_renderable->isBufferable())
		{
			BuffWriteResult res = _renderable->writeVertexData(batchingVertexBufferId);
			while (!res.operComplete)
			{
				flushBufferedRenderables();
				res = _renderable->writeVertexData(batchingVertexBufferId, res.nextSpriteIndex);
			}
		}
		else //non-bufferable
		{
			BuffWriteResult res({ 0,false });
			do
			{
				res = _renderable->writeVertexData(batchingVertexBufferId, res.nextSpriteIndex);
				flushBufferedRenderables();
			} while (res.nextSpriteIndex);
		}

	};

}
