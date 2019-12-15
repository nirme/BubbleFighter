#include "SceneManager.h"



namespace core
{
	namespace _2d
	{

		void SceneManager::addNode(SceneNode* _node)
		{
			allNodesList.push_back(_node);
			namedNodes.emplace(_node->getName(), _node);
		};


		void SceneManager::removeNode(SceneNode* _node)
		{
			SceneNodeList::iterator it = std::find(allNodesList.begin(), allNodesList.end(), _node);
			std::swap(*it, allNodesList.back());
			allNodesList.pop_back();

			namedNodes.erase(_node->getName());
		};


		SceneNode *SceneManager::getNodeByName(const std::string &_name)
		{
			assert(_name.size() && "node name string cannot be empty");

			for (unsigned int i = 0, iEnd = allNodesList.size(); i < iEnd; ++i)
			{
				if (allNodesList[i]->getName().compare(_name) == 0)
					return allNodesList[i];
			}

			return nullptr;
		};


		void SceneManager::findVisibleRenderables()
		{
			sceneRoot->findVisibleRenderables(currentCamera.get(), renderQueue.get(), &(currentViewport->getFrustrum()));
		};



		SceneManager::SceneManager() :
			renderSystem(nullptr)
		{};



		void SceneManager::setupManager(RenderSystem *_renderSystem, unsigned int _renderTargetWidth, unsigned int _renderTargetHeight, float _sceneScale)
		{
			assert(!renderSystem && "scene manager already setup");

			renderSystem = _renderSystem;

			// don't use std::make_unique, runtime issues with memory allocation in some cases

			std::unique_ptr<RenderQueue> renderQueueTmp(new RenderQueue());
			renderQueue.swap(renderQueueTmp);

			std::unique_ptr<SceneNode> sceneRootTmp(new SceneNode("root"));
			sceneRoot.swap(sceneRootTmp);

			std::unique_ptr<ViewPort> currentViewportTmp(new ViewPort(_renderTargetWidth, _renderTargetHeight, _sceneScale));
			currentViewport.swap(currentViewportTmp);

			std::unique_ptr<Camera> currentCameraTmp(new Camera("camera", currentViewport.get()));
			currentCamera.swap(currentCameraTmp);

			std::unique_ptr<ObjectFactory> objectFactoryTmp;
			objectFactory.swap(objectFactoryTmp);


			currentViewport->setScreenSize(_renderTargetWidth, _renderTargetHeight);
			currentViewport->setScale(_sceneScale);

			sceneRoot->appendChild(currentCamera.get());

			renderQueueTmp.release();
			sceneRootTmp.release();
			currentViewportTmp.release();
			currentCameraTmp.release();
			objectFactoryTmp.release();
		};


		SceneNode *SceneManager::createNode(const std::string &_nodeName, ScriptNodePtr _nodeValues)
		{
			SceneNode *newNode = new SceneNode(this, _nodeName);

			addNode(newNode);

			ScriptLoader &scriptLoader = ScriptLoader::getSingleton();
			newNode->setScale(scriptLoader.parseNodeScale(_nodeValues));
			newNode->setRotation(scriptLoader.parseNodeRotation(_nodeValues));
			newNode->setPosition(scriptLoader.parseNodePosition(_nodeValues));

			return newNode;
		};



		void SceneManager::destroyNode(SceneNode *_node)
		{
			removeNode(_node);
			_node->getParent()->removeChild(_node);
			delete _node;
		};




		void SceneManager::renderScene()
		{

			findVisibleRenderables();



			paramsManager.setCamera(currentCamera.get());
			paramsManager.setViewPort(currentViewport.get());

			// prepare camera and other scene related vars

			//renderSystem->getStateCashe().setVertexBuffer();
			//GraphicBuffer& vertexBuffer = renderSystem->getBatchingVertexBuffer().getId();
			//GraphicBuffer& indexBuffer = renderSystem->getIndexVertexBuffer();


			const RenderQueue::RenderableListByMaterial *rendMList = nullptr;
			MaterialManager &materialManager = MaterialManager::getSingleton();

			while ((rendMList = renderQueue->getNextPriorityRenderableList()))
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
							paramsManager.setCurrentMaterial(material.get());
						}
						else
						{
							paramsManager.setCurrentRenderable(renderableList[i]);
						}

						renderSystem->render(renderableList[i], material->program.get(), &paramsManager);
					}

					// flush anything bufferable after completing material loop
					paramsManager.setCurrentMaterial(material.get());
					renderSystem->flushRenderOp(material->program.get(), &paramsManager);
				}
			}

			renderQueue->clear();
		};




	}
}