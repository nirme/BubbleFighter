#pragma once

#include "SceneNode.h"
#include "RenderQueue.h"
#include "../RenderSystem.h"
#include "../GraphicBuffer.h"


namespace core
{
	namespace _2d
	{


		class SceneManager
		{
			RenderQueue renderQueue;
			RenderSystem *renderSystem;
			ShadingParamsPassthru *paramsManager;

		public:


			void renderScene()
			{

				// prepare camera and other scene related vars


				//update camera etc to params manager
				//paramsManager








				//////////////////////////////////////////

				const RenderQueue::RenderableListByMaterial *rendMList = nullptr;
				MaterialManager &materialMan = MaterialManager::getSingleton();

				while (rendMList = renderQueue.getNextPriorityRenderableList())
				{
					for (auto it = rendMList->begin(); it != rendMList->end(); ++it)
					{
						MaterialPtr material = materialMan.getById((*it).first);

						const ShadingProgramParams &params = material->program->getParams();

						renderSystem->useProgram(material->program);
						renderSystem->useTexture(0, material->texture);


						const RenderQueue::RenderableList &renderableList = (*it).second;
						bool bufferedRender(false);

						for (unsigned int i = 0; i < renderableList.size(); ++it)
						{
							if (renderableList[i]->isBufferable())
							{
								if (!bufferedRender)
								{
									// buffered renderables cannot have any custom params so we apply shared values beforehand
									paramsManager->setCurrentRenderable(nullptr);
									params.applyUniformValues(paramsManager);

									bufferedRender = true;
								}
							}
							else
							{
								//flush any buffered renderables
								if (bufferedRender)
								{
									renderSystem->flushBufferedRenderables();
									bufferedRender = false;
								}

								//set params for this renderable
								paramsManager->setCurrentRenderable(renderableList[i]);
								params.applyUniformValues(paramsManager);
							}
							renderSystem->render(renderableList[i]);
						}

						// flush anything in the buffer after completing loop
						renderSystem->flushBufferedRenderables();
					}


				}


			};



		};
	}
}