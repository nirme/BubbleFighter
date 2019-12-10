#pragma once

#include <thread>
#include <memory>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <vector>

#include "Exceptions.h"
#include "Logger.h"

#include "RenderSystem.h"

#include "ControllerManager.h"

#include "DataProvider.h"
#include "AndroidDataProvider.h"

#include "ScriptParser.h"
#include "XMLScriptParser.h"

#include "ScriptLoader.h"


#include "ResourceSystem.h"


#include "ShadingProgramManager.h"
#include "ImageSpriteManager.h"
#include "SpritedFontManager.h"
#include "TextureManager.h"

#include "GraphicBuffer.h"
#include "ShadingParamsPassthru.h"
#include "_2d/MaterialManager.h"

#include "_2d/ViewPort.h"
#include "Timer.h"




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


		//movr to renderSys
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t screenWwidth;
		int32_t screenHeight;









SCENE MANAGER
	| 
animation list





FRAME STARTED LISTENER







GraphicBufferPtr buffer;
Timer timer;
Timer::TimeStamp last;
Timer::TimeStamp frame;
double frametime;
unsigned int frames;


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

            ShadingProgramPtr program = ShadingProgramManager::getSingleton().getByName("baseShader");
            SpritedFontPtr font = SpritedFontManager::getSingleton().getByName("Lucida.xml");

            ImageSpritePtr sprite = ImageSpriteManager::getSingleton().getByName("anim_3.bmp");
            TexturePtr texture = TextureManager::getSingleton().getByName("anim_3.bmp");

            program->load();
            texture->load();

            font->load();

            sprite->load();

            static float vectorWidth, vectorHeight;
            static SpritedText spritedText;
            static GraphicBufferPtr textBuff;
            static GraphicBufferPtr indexes;
            std::string text = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890`-=[]\\;',./~!@#$%^&*()_+{}|:\"<>? ";

            if (!buffer)
            {
                spritedText = font->generateSpritedVector(text.c_str(), &vectorWidth, &vectorHeight, 16.0);

                float *tt = reinterpret_cast<float*>(spritedText.data());
                textBuff = GraphicBufferPtr(new GraphicBuffer(GL_STATIC_DRAW, GL_ARRAY_BUFFER, GL_FLOAT));
                textBuff->resize(spritedText.size() * 8 * 2);
                textBuff->load();
                textBuff->write(reinterpret_cast<const float*>(spritedText.data()), spritedText.size() * 14);
                textBuff->uploadData();

                unsigned short *ind = new unsigned short [spritedText.size() * 6];

                for (int i=0; i <spritedText.size(); ++i)
                {
                    ind[i*6 + 0] = i*4 + 0;
                    ind[i*6 + 1] = i*4 + 2;
                    ind[i*6 + 2] = i*4 + 3;
                    ind[i*6 + 3] = i*4 + 0;
                    ind[i*6 + 4] = i*4 + 3;
                    ind[i*6 + 5] = i*4 + 1;
                }

                indexes = GraphicBufferPtr(new GraphicBuffer(GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_SHORT));
                indexes->resize(spritedText.size() * 6);
                indexes->load();
                indexes->write(ind, spritedText.size() * 6);
                indexes->uploadData();


                delete [] ind;





/*
   0---1
1  | \ |  2
   2---3

   0,2,3
   0,3,1

*/
/*
                for (int i=0; i<5; ++i)
                {
                    // i=0
                    data[0] = SpriteCoords::SPRITE_SQUARE.uvPoints[0].x + base;
                    data[1] = SpriteCoords::SPRITE_SQUARE.uvPoints[0].y + base;
                    data[2] = coords.uvPoints[0].x;
                    data[3] = coords.uvPoints[0].y;

                    // i=1
                    data[4] = SpriteCoords::SPRITE_SQUARE.uvPoints[1].x + base;
                    data[5] = SpriteCoords::SPRITE_SQUARE.uvPoints[1].y + base;
                    data[6] = coords.uvPoints[1].x;
                    data[7] = coords.uvPoints[1].y;

                    // i=2
                    data[8] = SpriteCoords::SPRITE_SQUARE.uvPoints[2].x + base;
                    data[9] = SpriteCoords::SPRITE_SQUARE.uvPoints[2].y + base;
                    data[10] = coords.uvPoints[2].x;
                    data[11] = coords.uvPoints[2].y;

                    // i=3
                    data[12] = SpriteCoords::SPRITE_SQUARE.uvPoints[3].x + base;
                    data[13] = SpriteCoords::SPRITE_SQUARE.uvPoints[3].y + base;
                    data[14] = coords.uvPoints[3].x;
                    data[15] = coords.uvPoints[3].y;

                    buffer->write(data.data(),4*4);

                    base += 2.0f;

                }
*/

/*
                // i=0
                data[4*0 + 0] = SpriteCoords::SPRITE_SQUARE.uvPoints[0].x;
                data[4*0 + 1] = SpriteCoords::SPRITE_SQUARE.uvPoints[0].y;
                data[4*0 + 2] = coords.uvPoints[0].x;
                data[4*0 + 3] = coords.uvPoints[0].y;

                // i=1
                data[4*1 + 0] = SpriteCoords::SPRITE_SQUARE.uvPoints[1].x;
                data[4*1 + 1] = SpriteCoords::SPRITE_SQUARE.uvPoints[1].y;
                data[4*1 + 2] = coords.uvPoints[1].x;
                data[4*1 + 3] = coords.uvPoints[1].y;

                // i=2
                data[4*2 + 0] = SpriteCoords::SPRITE_SQUARE.uvPoints[2].x;
                data[4*2 + 1] = SpriteCoords::SPRITE_SQUARE.uvPoints[2].y;
                data[4*2 + 2] = coords.uvPoints[2].x;
                data[4*2 + 3] = coords.uvPoints[2].y;

                // i=3
                data[4*3 + 0] = SpriteCoords::SPRITE_SQUARE.uvPoints[3].x;
                data[4*3 + 1] = SpriteCoords::SPRITE_SQUARE.uvPoints[3].y;
                data[4*3 + 2] = coords.uvPoints[3].x;
                data[4*3 + 3] = coords.uvPoints[3].y;


                buffer->write(&data[0*4],4);
                buffer->write(&data[2*4],4);
                buffer->write(&data[3*4],4);
                buffer->write(&data[0*4],4);
                buffer->write(&data[3*4],4);
                buffer->write(&data[1*4],4);
*/

            }


            static Vector3 color = {1.0f, 0.0f, 0.0f};
            static int index = 1;

            if (color.v[index] > 0.9999f)
            {
                color.v[(index+2) % 3] = 0.0f;
                index = ++index % 3;
            }

            color.v[index] += 0.02f;
            color.v[(index+2) % 3] -= 0.02f;


            GLenum err;

            glClearColor(color.x, color.y, color.z, 1.0f);
            err = glGetError();

            glClear(GL_COLOR_BUFFER_BIT);
            err = glGetError();


            glUseProgram(program->getId());
            err = glGetError();

            _2d::MaterialManager::getSingleton().generateMaterial(program, texture);

            ShadingParamsPassthru paramsStore;

            _2d::ViewPort viewPort;
            viewPort.setScreenSize(width,height);
            viewPort.setScale(0.5f);
            paramsStore.setViewPort(&viewPort);

            //paramsStore.setCurrentMaterial(_2d::MaterialManager::getSingleton().generateMaterial(program, texture).get());
            paramsStore.setCurrentMaterial(_2d::MaterialManager::getSingleton().generateMaterial(program, font->getTexture()).get());

            const ShadingProgramParams &params = program->getParams();
            params.applyUniformValues(&paramsStore);


            const ShadingProgram::VertexAttribList &att = program->getAttribList();


            //paramsStore.setCurrentMaterial(_2d::MaterialManager::getSingleton().generateMaterial(program, font->getTexture()).get());
            //params.applyUniformValues(&paramsStore);

            //glBindBuffer(GL_ARRAY_BUFFER, textBuff->getId());
            //err = glGetError();

            //glBindBuffer(GL_ARRAY_BUFFER, buffer->getId());
            glBindBuffer(GL_ARRAY_BUFFER, textBuff->getId());
            err = glGetError();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes->getId());
            err = glGetError();


            for (int i = 0; i < att.size(); ++i)
            {
                // 1st attribute buffer : vertices
                glEnableVertexAttribArray(att[i].id);
                err = glGetError();

                glVertexAttribPointer(
                        att[i].id,          // attribute 0. No particular reason for 0, but must match the layout in the shader.
                        att[i].size,                  // size
                        att[i].type,           // type
                        GL_FALSE,           // normalized?
                        att[i].stride,                  // stride
                        (void *) att[i].offsetInBytes            // array buffer offset
                );
                err = glGetError();

            }

            // Draw the triangle !
            //glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
            //err = glGetError();



            //glDrawElements(GL_TRIANGLES, 6*5, GL_UNSIGNED_SHORT, nullptr);
            glDrawElements(GL_TRIANGLES, text.size()*6, GL_UNSIGNED_SHORT, nullptr);

            //glDrawArrays(GL_TRIANGLES, 0, spritedText.size() * 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
            err = glGetError();





            glDisableVertexAttribArray(0);
            err = glGetError();


            if (eglSwapBuffers(display, surface) != EGL_TRUE)
			    return;



timer.update();

double time = timer.timeElapsed(&last);
double frameTime = timer.timeElapsed(&frame);
//Logger::getSingleton().write(std::string("frame time: ") + std::to_string(frameTime));
if (time >= 1.0)
{
    double fps = timer.updatesElapsed(&last) / time;
    Logger::getSingleton().write(std::string("FPS: ") + std::to_string(fps));
    last = timer.getTimeStamp();
}

frame = timer.getTimeStamp();
		};



        EGLConfig getMatchigConfig(const EGLint *attributes)
        {
            EGLint numConfigs;

            EGL_ERROR_CHECK(eglChooseConfig(display, attributes, nullptr, 0, &numConfigs));
            std::vector<EGLConfig> configs(numConfigs);
            EGL_ERROR_CHECK(eglChooseConfig(display, attributes, configs.data(), numConfigs, &numConfigs));

            EGLint attrValue;


            for (int i = 0; i < numConfigs; ++i)
            {
                const EGLint *attributePair = attributes;
                EGLint attributeValue;

                bool configMatching = true;
                while (attributePair[0] != EGL_NONE)
                {
                    EGL_ERROR_CHECK(eglGetConfigAttrib(display, configs[i], attributePair[0], &attributeValue));

                    if (attributePair[1] != (attributeValue & attributePair[1]))
                    {
                        configMatching = false;
                        break;
                    }

                    attributePair += 2;
                }

                if (configMatching) return configs[i];
            }

            throw std::runtime_error("No frame buffer configuration matching selected attributes");
        };



		void onInitWindow()
		{
			// setup parser and data provider with specific impl

			loader.registerParser(std::make_shared<XmlScriptParser>());
			appDataProvider = std::make_shared<AndroidDataProvider>(androidApp->activity->assetManager);

			resourceManager.registerDataProvider(appDataProvider);


			// initialize system required for resource loading
			renderer.initialize();

			// register res managers and parse configuration

			resourceManager.registerResourceManager("texture", new TextureManager);
			resourceManager.registerResourceManager("shader", new ShaderManager);
			resourceManager.registerResourceManager("shading_program", new ShadingProgramManager);
			resourceManager.registerResourceManager("sprite", new ImageSpriteManager);
			resourceManager.registerResourceManager("font", new SpritedFontManager);

			resourceManager.parseConfiguration("GameResourceCfg.xml");







/*

            const EGLint attribs[] = {
                    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                    EGL_BLUE_SIZE, 8,
                    EGL_GREEN_SIZE, 8,
                    EGL_RED_SIZE, 8,
                    EGL_ALPHA_SIZE, 0, //no alpha for render end surface
                    EGL_BUFFER_SIZE, 24, //RGB8 = 24
                    EGL_CONFORMANT, EGL_OPENGL_ES2_BIT, //GLES2
					EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //GLES2
                    EGL_NONE
            };

            display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

            EGLint major, minor;
			eglInitialize(display, &major, &minor);


            EGLint w, h, format;

            EGLConfig config = getMatchigConfig(attribs);

			eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

			ANativeWindow_setBuffersGeometry(androidApp->window, 0, 0, format);

			surface = eglCreateWindowSurface(display, config, androidApp->window, NULL);

            const EGLint context_attribs[] = {
                    EGL_CONTEXT_MAJOR_VERSION, 2, //GLES2
                    EGL_NONE
            };

            context = eglCreateContext(display, config, NULL, context_attribs);

			if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
				//LOGW("Unable to eglMakeCurrent");
				return;
			}

			eglQuerySurface(display, surface, EGL_WIDTH, &w);
			eglQuerySurface(display, surface, EGL_HEIGHT, &h);

			width = w;
			height = h;



timer.reset();
last = timer.getTimeStamp();
//eglSwapInterval(display, 0 );

			// Initialize GL state.
			//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
            glDisable(GL_CULL_FACE);

            glEnable(GL_BLEND);

            //glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
            //glBlendFunc(GL_ONE, GL_ZERO);
            //glBlendFunc(GL_ZERO, GL_ONE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glShadeModel(GL_SMOOTH);
			glDisable(GL_DEPTH_TEST);



*/

            initialized = true;
		};


		void onTermWindow()
		{
		
			if (display != EGL_NO_DISPLAY) {
				eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
				if (context != EGL_NO_CONTEXT) {
					eglDestroyContext(display, context);
				}
				if (surface != EGL_NO_SURFACE) {
					eglDestroySurface(display, surface);
				}
				eglTerminate(display);
			}
			display = EGL_NO_DISPLAY;
			context = EGL_NO_CONTEXT;
			surface = EGL_NO_SURFACE;

            initialized = false;
		};

		void onGainedFocus() {};

		void onLostFocus() {};
		void onConfigChanged() {};

		void onLowMemory() {};
		void onStart()
		{};


		void onResume(void* pData, size_t pSize) {};

		void onSaveState(void** pData, size_t* pSize) {};
		void onPause() {};
		void onStop() {};
		void onDestroy() {};

	};

}
