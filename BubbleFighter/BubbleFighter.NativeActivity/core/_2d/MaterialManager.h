#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <array>
#include "../SingletonTemplate.h"

#include "../ShadingProgram.h"
#include "../Texture.h"


namespace core
{
	namespace _2d
	{
		typedef unsigned long int MaterialId;
		struct Material
		{
			MaterialId id;
			ShadingProgramPtr program;
			TexturePtr texture;
		};

		typedef std::shared_ptr<const Material> MaterialPtr;

		class MaterialManager : public Singleton<MaterialManager>
		{
		protected:
			typedef std::unordered_map<std::pair<ResourceHandle, ResourceHandle>, MaterialPtr> MaterialIdList;
			typedef std::unordered_map<MaterialId, MaterialPtr> MaterialDefByIdList;

			MaterialIdList materialIdsList;
			MaterialDefByIdList materialsByIds;

			MaterialId nextFreeId;

		public:

			MaterialManager() :
				nextFreeId(1)
			{};


			MaterialPtr generateMaterial(ShadingProgramPtr _program, TexturePtr _texture)
			{
				auto it = materialIdsList.find(std::pair(_program->getHandle(), _texture->getHandle()));
				if (it != materialIdsList.end())
					return (*it).second;

				MaterialPtr material = std::make_shared<const Material>(nextFreeId++, _program, _texture);
				auto newEntry = materialIdsList.emplace(std::pair(_program->getHandle(), _texture->getHandle()), material);

				materialsByIds.emplace(material->id, material);
				return material;
			};

			MaterialPtr getById(MaterialId _id)
			{
				auto it = materialsByIds.find(_id);
				return it != materialsByIds.end() ? (*it).second : nullptr;
			};

		};
	}
}
