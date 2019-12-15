#pragma once

#include "../ScriptNode.h"
#include "../ScriptLoader.h"

#include "SingleSprite.h"

#include "../ShadingProgramManager.h"
#include "../ImageSpriteManager.h"



namespace core
{

	enum OBJECT_TYPE
	{
		OT_UNDEFINED,
		OT_SINGLE_SPRITE,
	};


	class ObjectFactory
	{
	public:

		_2d::SingleSpriteUPtr createSingleSprite(const std::string &_name, ScriptNodePtr _scriptNode)
		{
			ScriptLoader &scriptLoader = ScriptLoader::getSingleton();

			// keep object as unique ptr until another object take resposibility for it
			_2d::SingleSpriteUPtr object(new _2d::SingleSprite(_name));

			object->setMaterial(
				ShadingProgramManager::getSingleton().getByName(scriptLoader.parseObjectShader(_scriptNode)),
				ImageSpriteManager::getSingleton().getByName(scriptLoader.parseObjectImageSprite(_scriptNode))
			);

			object->setScale(scriptLoader.parseObjectScale(_scriptNode));
			object->setRotation(scriptLoader.parseObjectRotation(_scriptNode));
			object->setPosition(scriptLoader.parseObjectPosition(_scriptNode));

			object->setPriority(scriptLoader.parseRenderablePriority(_scriptNode));
			object->setSpriteCoords(scriptLoader.parseSingleSpriteCoords(_scriptNode));

			
			return object;
		};

		_2d::MovableObjectUPtr createObject(ScriptNodePtr _scriptNode)
		{
			ScriptLoader &scriptLoader = ScriptLoader::getSingleton();
			std::string name = scriptLoader.parseObjectName(_scriptNode);
			OBJECT_TYPE type = scriptLoader.parseRenderableType(_scriptNode);

			switch (type)
			{
			case OT_SINGLE_SPRITE:
				return createSingleSprite(name, _scriptNode);
			}

			return _2d::MovableObjectUPtr(nullptr);
		};


	};
}