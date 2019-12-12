#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "ViewPort.h"

#include "SceneNode.h"
#include "Entity.h"
#include "RenderQueue.h"
#include "../ShadingParamsPassthru.h"
#include "../RenderSystem.h"
#include "../GraphicBuffer.h"


namespace core
{
	namespace _2d
	{


		class SceneManager
		{
		protected:

			RenderQueue renderQueue;
			RenderSystem *renderSystem;
			ShadingParamsPassthru *paramsManager;

			
			std::unique_ptr<Camera> currentCamera;
			std::unique_ptr<ViewPort> currentViewport;

			std::unique_ptr<SceneNode> sceneRoot;


			typedef std::vector<SceneNode*> SceneNodeList;
			SceneNodeList allNodesList;


			SceneNode *getNodeByName(const char *_name)
			{
				assert(_name && "node name string cannot be null");

				for (unsigned int i = 0, iEnd = allNodesList.size(); i < iEnd; ++i)
				{
					if (allNodesList[i]->getName().compare(_name) == 0)
						return allNodesList[i];
				}

				return nullptr;
			};

		public:

			SceneManager() :
				renderSystem(nullptr), 
				paramsManager(nullptr),
				currentCamera(nullptr),
				currentViewport(nullptr),
				sceneRoot(nullptr)
			{};



			void setupManager(unsigned int _renderTargetWidth, unsigned int _renderTargetHeight, float _sceneScale = 1.0f)
			{
				sceneRoot = std::make_unique<SceneNode>("root");

				currentViewport = std::make_unique<ViewPort>();

				currentViewport->setScreenSize(_renderTargetWidth, _renderTargetHeight);
				currentViewport->setScale(_sceneScale);

				currentCamera = std::make_unique<Camera>("camera", currentViewport.get());

				sceneRoot->appendChild(currentCamera.get());
			};


			SceneNode *createSceneNode(const char *_parent, const char *_nodeName)
			{
				SceneNode *parentNode = nullptr;

				if (!_parent || !(parentNode = getNodeByName(_parent)))
					parentNode = sceneRoot.get();

				SceneNode *node = new SceneNode(_nodeName);
				allNodesList.push_back(node);

				parentNode->appendChild(node);
				return node;
			};



			Entity *createEntity(const char *_parent, const char *_nodeName)
			{
				SceneNode *parentNode = nullptr;

				if (!_parent || !(parentNode = getNodeByName(_parent)))
					parentNode = sceneRoot.get();

				Entity *node = new Entity(_nodeName);
				allNodesList.push_back(node);

				parentNode->appendChild(node);
				return node;
			};





			void findVisibleRenderables()
			{
				sceneRoot->findVisibleRenderables(currentCamera.get(), &renderQueue, &(currentViewport->getFrustrum()));
			};


			void renderScene()
			{

				findVisibleRenderables();



				paramsManager->setCamera(currentCamera.get());
				paramsManager->setViewPort(currentViewport.get());

				// prepare camera and other scene related vars

				//renderSystem->getStateCashe().setVertexBuffer();
				//GraphicBuffer& vertexBuffer = renderSystem->getBatchingVertexBuffer().getId();
				//GraphicBuffer& indexBuffer = renderSystem->getIndexVertexBuffer();


				const RenderQueue::RenderableListByMaterial *rendMList = nullptr;
				MaterialManager &materialManager = MaterialManager::getSingleton();

				while (rendMList = renderQueue.getNextPriorityRenderableList())
				{
					for (auto it = rendMList->begin(); it != rendMList->end(); ++it)
					{
						if (!(*it).second.size())
							continue;

						// get material from first renderable, all in this list use the same
						MaterialPtr material = (*it).second.front()->getMaterial();
						const ShadingProgramParams *params = material->program->getParams();


						// setup gpu state cashe with values for this queued list
						renderSystem->getStateCashe().setShadingProgram(material->program->getId());
						renderSystem->getStateCashe().setVertexAtribCount(material->program->getAttribList().size());


						for (int t = 0, tEnd = material->textures.size(); t < tEnd; ++t)
							renderSystem->getStateCashe().setActiveTexture(t, material->textures[t]->getId());


						const RenderQueue::RenderableList &renderableList = (*it).second;
						bool bufferedRender(false);

						for (unsigned int i = 0; i < renderableList.size(); ++it)
						{

							if (renderableList[i]->isBufferable())
							{
								// bufferables share settings in material so we set only material in passthrough object zeroing renderable pointer
								paramsManager->setCurrentMaterial(material.get());
							}
							else
							{
								paramsManager->setCurrentRenderable(renderableList[i]);
							}

							renderSystem->render(renderableList[i], material->program.get(), paramsManager);
						}

						// flush anything bufferable after completing material loop
						paramsManager->setCurrentMaterial(material.get());
						renderSystem->flushRenderOp(material->program.get(), paramsManager);
					}
				}

				renderQueue.clear();

			};



		};
	}
}