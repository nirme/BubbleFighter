#pragma once


#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <vector>

#include "Exceptions.h"
#include "Logger.h"

#include "ShadingProgramManager.h"
#include "ImageSpriteManager.h"
#include "SpritedFontManager.h"
#include "TextureManager.h"

#include "GraphicBuffer.h"
#include "ShadingParamsPassthru.h"
#include "_2d/MaterialManager.h"

//	APP_CMD_INPUT_CHANGED	//	used by app glue internaly
//	APP_CMD_WINDOW_RESIZED	//	unused, app fullscreen only
//	APP_CMD_WINDOW_REDRAW_NEEDED	//	unused/don't care
//	APP_CMD_CONTENT_RECT_CHANGED	//	consider using, soft buttons could interfere

/*
APP_CMD_INIT_WINDOW	//	new ANativeWindow is ready for use. Upon receiving this command, android_app->window will contain the new window surface
APP_CMD_TERM_WINDOW	//	existing ANativeWindow needs to be terminated. Upon receiving this command, android_app->window still contains the existing window; after calling android_app_exec_cmd it will be set to NULL
APP_CMD_GAINED_FOCUS	//	app's activity window has gained input focus
APP_CMD_LOST_FOCUS	//	app's activity window has lost input focus
APP_CMD_CONFIG_CHANGED	//	Command from main thread: the current device configuration has changed
APP_CMD_LOW_MEMORY	//	the system is running low on memory, try to reduce your memory use
APP_CMD_START	//	app's activity has been started
APP_CMD_RESUME	//	app's activity has been resumed
APP_CMD_SAVE_STATE	//	app should generate a new saved state for itself, to restore from later if needed. If you have saved state, allocate it with malloc and place it in android_app.savedState with the size in android_app.savedStateSize. The will be freed for you later
APP_CMD_PAUSE	//	the app's activity has been paused
APP_CMD_STOP	//	app's activity has been stopped
APP_CMD_DESTROY	//	app's activity is being destroyed and waiting for the app thread to clean up and exit before proceeding
*/



namespace core
{

	class Engine
	{
	private:
		android_app* androidApp;

		bool initialized;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;

		float angle;

GraphicBufferPtr buffer;


	public:

		Engine(android_app* _androidApp) :
			androidApp(_androidApp), angle(0.0f), initialized(false)
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

            //font->load();

            //sprite->load();


            if (!buffer)
            {
                buffer = GraphicBufferPtr(new GraphicBuffer(GL_STATIC_DRAW, GL_ARRAY_BUFFER, GL_FLOAT));
                //buffer = GraphicBufferPtr(new GraphicBuffer(GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER, GL_FLOAT));
                buffer->resize(24);
                buffer->load();

                std::vector<float> data(16);
                //const SpriteCoords& coords = sprite->getCoords();

/*
   0---1
1  | \ |  2
   2---3

   0,2,3
   0,3,1

*/

                // i=0
                data[4*0 + 0] = -1.0f;
                data[4*0 + 1] = 1.0f;
                data[4*0 + 2] = 0.0f; //coords.uvPoints[0].x;
                data[4*0 + 3] = 1.0f; //coords.uvPoints[0].y;

                // i=1
                data[4*1 + 0] = 1.0f;
                data[4*1 + 1] = 1.0f;
                data[4*1 + 2] = 1.0f; //coords.uvPoints[1].x;
                data[4*1 + 3] = 1.0f; //coords.uvPoints[1].y;

                // i=2
                data[4*2 + 0] = -1.0f;
                data[4*2 + 1] = -1.0f;
                data[4*2 + 2] = 0.0f; //coords.uvPoints[2].x;
                data[4*2 + 3] = 0.0f; //coords.uvPoints[2].y;

                // i=3
                data[4*3 + 0] = 1.0f;
                data[4*3 + 1] = -1.0f;
                data[4*3 + 2] = 1.0f; //coords.uvPoints[3].x;
                data[4*3 + 3] = 0.0f; //coords.uvPoints[3].y;


                buffer->write(&data[0*4],4);
                buffer->write(&data[2*4],4);
                buffer->write(&data[3*4],4);
                buffer->write(&data[0*4],4);
                buffer->write(&data[3*4],4);
                buffer->write(&data[1*4],4);


/*
                // i=0
                data[2*0 + 0] = -1.0f;
                data[2*0 + 1] = 1.0f;

                // i=1
                data[2*1 + 0] = 1.0f;
                data[2*1 + 1] = 1.0f;

                // i=2
                data[2*2 + 0] = -1.0f;
                data[2*2 + 1] = -1.0f;

                // i=3
                data[2*3 + 0] = 1.0f;
                data[2*3 + 1] = -1.0f;




                buffer->write(&data[0*2],2);
                buffer->write(&data[2*2],2);
                buffer->write(&data[3*2],2);

                buffer->write(&data[0*2],2);
                buffer->write(&data[3*2],2);
                buffer->write(&data[1*2],2);
*/

                buffer->uploadData();

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
/*
            glUseProgram(program->getId());
            err = glGetError();

            const ShadingProgram::VertexAttribList &att = program->getAttribList();

            for (unsigned int i = 0; i < att.size(); ++i)
            {

                glEnableVertexAttribArray(att[i].id);
                err = glGetError();

                glVertexAttribPointer(att[i].id, att[i].size, att[i].type, GL_FALSE, att[i].stride, (const GLvoid*) att[i].offsetInBytes );
                err = glGetError();

            }


            glActiveTexture(GL_TEXTURE0);
            err = glGetError();

            glBindTexture(GL_TEXTURE_2D, sprite->getTexture()->getId());
            err = glGetError();


            glBindBuffer(GL_ARRAY_BUFFER, buffer->getId());
            err = glGetError();

            glDrawArrays(GL_TRIANGLES, 0, 2);
            err = glGetError();

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            err = glGetError();
*/




/*
            static GLuint vertexbuffer = 0;

            // This will identify our vertex buffer
            //GLuint vertexbuffer;
            // Generate 1 buffer, put the resulting identifier in vertexbuffer
            //static GLfloat g_vertex_buffer_data[] = {};
            static GLfloat g_vertex_buffer_data[] = {-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};

            if (vertexbuffer == 0)
            {
                glGenBuffers(1, &vertexbuffer);
                err = glGetError();

                // The following commands will talk about our 'vertexbuffer' buffer
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                err = glGetError();

                // Give our vertices to OpenGL.
                glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
                err = glGetError();
            } else
            {
                static bool dir = false;

                if (!dir )
                {
                    g_vertex_buffer_data[0] += 0.02;
                    g_vertex_buffer_data[1] += 0.02;
                    g_vertex_buffer_data[4] += 0.02;
                    g_vertex_buffer_data[5] += 0.02;
                    g_vertex_buffer_data[8] += 0.02;
                    g_vertex_buffer_data[9] += 0.02;


                    if (g_vertex_buffer_data[4] > 1.0f)
                        dir = true;
                }
                else
                {
                    g_vertex_buffer_data[0] -= 0.02;
                    g_vertex_buffer_data[1] -= 0.02;
                    g_vertex_buffer_data[4] -= 0.02;
                    g_vertex_buffer_data[5] -= 0.02;
                    g_vertex_buffer_data[8] -= 0.02;
                    g_vertex_buffer_data[9] -= 0.02;

                    if (g_vertex_buffer_data[4] < 0.0f)
                        dir = false;
                }

                glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(g_vertex_buffer_data), g_vertex_buffer_data);
            }
*/

            glUseProgram(program->getId());
            err = glGetError();

            _2d::MaterialManager::getSingleton().generateMaterial(program, texture);

            ShadingParamsPassthru paramsStore;
            paramsStore.setCurrentMaterial(_2d::MaterialManager::getSingleton().generateMaterial(program, texture).get());

            const ShadingProgramParams &params = program->getParams();
            params.applyUniformValues(&paramsStore);




/*
            try {
                ShadingProgramParams::UniformList *unif = &(program->getParamList()->programUniforms);

                for (int i = 0; i < unif->size(); ++i) {
                    if (unif->at(i).name.compare("texture_0") == 0) {
                        GLint unit = UU_SAMPLER_0 - UU_SAMPLER_0;
                        glUniform1i(unif->at(i).id, unit);
                        err = glGetError();

                        glActiveTexture(GL_TEXTURE0);
                        err = glGetError();

                        glBindTexture(GL_TEXTURE_2D, texture->getId());
                        err = glGetError();
                    }

                    if (unif->at(i).name.compare("viewProjMx3") == 0) {
                        float scale = 0.1f;
                        Matrix3 viewProj((height / width) * scale, 0.0f, 0.0f, 0.0f, scale, 0.0f,
                                         0.0f, 0.0f, 1.0f);
                        glUniformMatrix3fv(unif->at(i).id, 1, GL_FALSE, viewProj.m);
                        err = glGetError();
                    }

                }
            }
            catch (const std::exception &e)
            {
                Logger::getSingleton().write(e.what());
            }
*/


            const ShadingProgram::VertexAttribList &att = program->getAttribList();

            glBindBuffer(GL_ARRAY_BUFFER, buffer->getId());
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
            glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
            err = glGetError();

            glDisableVertexAttribArray(0);
            err = glGetError();



            if (eglSwapBuffers(display, surface) != EGL_TRUE)
			    return;

			angle += 0.01f;
			if (angle > 3.0f)
				angle = 0.0f;





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

/*
            const EGLint attribs[] = {
                    //EGL_SAMPLE_BUFFERS, 1,
                    //EGL_SAMPLES, 1,
                    EGL_NONE
            };
*/
            display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

            EGLint major, minor;
			eglInitialize(display, &major, &minor);

			/* Here, the application chooses the configuration it desires. In this
			* sample, we have a very simplified selection process, where we pick
			* the first EGLConfig that matches our criteria */

            EGLint w, h, format;

            EGLConfig config = getMatchigConfig(attribs);


			/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
			* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
			* As soon as we picked a EGLConfig, we can safely reconfigure the
			* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
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
			angle = 0.0f;

			// Initialize GL state.
			//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
            glEnable(GL_CULL_FACE);

            glEnable(GL_BLEND);

            //glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
            //glBlendFunc(GL_ONE, GL_ZERO);
            //glBlendFunc(GL_ZERO, GL_ONE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glShadeModel(GL_SMOOTH);
			glDisable(GL_DEPTH_TEST);

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
