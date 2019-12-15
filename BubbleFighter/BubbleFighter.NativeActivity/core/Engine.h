#pragma once

#include <thread>
#include <memory>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

//#include "Exceptions.h"
//#include "Logger.h"

#include "RenderSystem.h"

#include "_2d/SceneManager.h"

#include "ControllerManager.h"

#include "DataProvider.h"
#include "AndroidDataProvider.h"
#include "XMLScriptParser.h"
#include "ScriptLoader.h"


#include "ResourceSystem.h"

#include "ShadingProgramManager.h"
#include "ImageSpriteManager.h"
#include "SpritedFontManager.h"
#include "TextureManager.h"

#include "_2d/ViewPort.h"
#include "Timer.h"




//#include "_2d/Entity.h"





namespace core
{

	class Engine
	{
	private:
		android_app* androidApp;

		bool initialized;

		DataProviderPtr appDataProvider;
		ControllerManager controllerManager;
		ScriptLoader loader;
		ResourceSystem resourceManager;
		RenderSystem renderer;


		Timer frameTime;
		_2d::SceneManager mainScene;

	public:

		Engine(android_app* _androidApp) :
			androidApp(_androidApp), initialized(false)
		{};


		void onActivate() {};
		void onDeactivate() {};

        void onStep()
        {
            if (!initialized)
                return;

            // update all controllers
            frameTime.update();
            controllerManager.addFrameTime(frameTime.getLastUpdateTime<double>());
            controllerManager.updateControllers();



            // render scene
            mainScene.renderScene();
        };



		void onInitWindow()
		{
			// setup parser and data provider with specific impl

			loader.registerParser(std::make_shared<XmlScriptParser>());
			appDataProvider = std::make_shared<AndroidDataProvider>(androidApp->activity->assetManager);

			resourceManager.registerDataProvider(appDataProvider);


			// initialize systems required for resource loading
            renderer.enableMultisampling(true);
			renderer.initialize(androidApp);

			// register res managers and parse configuration

			resourceManager.registerResourceManager("texture", new TextureManager(&renderer));
			resourceManager.registerResourceManager("shader", new ShaderManager(&renderer));
			resourceManager.registerResourceManager("shading_program", new ShadingProgramManager(&renderer));
			resourceManager.registerResourceManager("sprite", new ImageSpriteManager);
			resourceManager.registerResourceManager("font", new SpritedFontManager);

			resourceManager.parseConfiguration("GameResourceCfg.xml");

			// init controllers
            controllerManager.initialize();

            mainScene.setupManager(&renderer, renderer.getScreenWidth(), renderer.getScreenHeight(), (float)renderer.BaseScreenDensity / renderer.getScreenDensity());

            _2d::Entity* bg = mainScene.createEntity("root", "background");

			bg


            frameTime.reset();
            initialized = true;

		};


		void onTermWindow()
		{

            renderer.uninitialize();

            initialized = false;
		};

		void onGainedFocus() {};

		void onLostFocus() {};
		void onConfigChanged() {};

		void onLowMemory() {};
		void onStart()
		{};


		void onResume(void* pData, size_t pSize)
		{

		};

		void onSaveState(void** pData, size_t* pSize) {};
		void onPause() {};

		void onStop()
		{
            resourceManager.releaseAllResourceSystems();
		};


		void onDestroy()
		{

		};

	};

}
