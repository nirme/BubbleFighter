#pragma once

#include <thread>
#include <memory>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "ActivityHandler.h"

//#include "Exceptions.h"
//#include "Logger.h"

#include "RenderSystem.h"

#include "_2d/SceneManager.h"

#include "ControllerManager.h"

#include "DataProvider.h"
#include "AndroidDataProvider.h"
#include "XMLScriptParser.h"
#include "ScriptLoader.h"

#include "InputManager.h"
#include "_2d/ShapeRectangle.h"
#include "TouchControls.h"

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

	class Engine : public ActivityHandler
	{
	private:
		android_app* androidApp;

		bool initialized;

		DataProviderPtr appDataProvider;
		ControllerManager controllerManager;
		ScriptLoader loader;
		ResourceSystem resourceManager;
		RenderSystem renderer;

		InputManager inputManager;


		Timer frameTime;
		_2d::SceneManager mainScene;

	public:

		Engine(android_app* _androidApp) :
			androidApp(_androidApp), initialized(false)
		{};


		InputManager *getInputManager()
		{
			return &inputManager;
		};



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

			inputManager.setScreenSize(renderer.getScreenWidth(), renderer.getScreenHeight());





{
	_2d::SceneNode *node = mainScene.createNode("TestNode", nullptr);
	mainScene.getNodeByName("root")->appendChild(node);

	node->setPosition({ 0.0f, 0.0f });
	node->setRotation(0.0f);
	node->setScale({ 1.0f });


	_2d::SingleSprite* spriteTest = mainScene.createSingleSprite("TestSprite", nullptr);
	spriteTest->setEnabled(true);
	spriteTest->setMaterial(ShadingProgramManager::getSingleton().getByName("baseShader"), ImageSpriteManager::getSingleton().getByName("anim_1.bmp"));
	spriteTest->setPosition({ 0.0f});
	spriteTest->setPriority(127);
	spriteTest->setRotation(0.0f);
	spriteTest->setScale({ 1.0f });
	spriteTest->setSpriteCoords(SpriteCoords::SPRITE_SQUARE);
	node->appendObject(spriteTest);


	class MoveListener : public TouchControl::Listener
	{
	public:
		_2d::MovableObject *object;
		Vector2 pos;
		void onPointerOnArea() { object->setPosition(object->getPosition() + pos); };
		void onPointerOffArea() { object->setPosition(object->getPosition() + pos); };
	};

	// btn1
	MoveListener *mList1 = new MoveListener();
	mList1->object = spriteTest;
	mList1->pos.x = -0.3f;

	_2d::Shape *btn1Shape = new _2d::Rectangle(Vector2(-10.0f, 10.0f), Vector2(-0.6f, 10.0f), Vector2(-0.6f, -10.0f), Vector2(-10.0f, -10.0f));
	TouchArea *buttonLeft = new TouchArea(btn1Shape);
	buttonLeft->registerListener(mList1);

	inputManager.registerControl("base_control", buttonLeft);


	// btn2
	MoveListener *mList2 = new MoveListener();
	mList2->object = spriteTest;
	mList2->pos.x = 0.3f;

	_2d::Shape *btn2Shape = new _2d::Rectangle(Vector2(10.0f, 10.0f), Vector2(0.6f, 10.0f), Vector2(0.6f, -10.0f), Vector2(10.0f, -10.0f));
	TouchArea *buttonRight = new TouchArea(btn2Shape);
	buttonRight->registerListener(mList2);

	inputManager.registerControl("base_control", buttonRight);

	inputManager.activateControlSet("base_control");

}



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
