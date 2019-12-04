#pragma once

#include <memory>
#include <utility>
#include <string>
#include <unordered_map>
#include <array>

#include "../ShadingProgram.h"
#include "../SingletonTemplate.h"

#include "../Texture.h"


namespace core
{
	namespace _2d
	{
		typedef unsigned int MaterialId;
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


			struct MaterialKey
			{
				ResourceHandle programHandle;
				ResourceHandle textureHandle;

				MaterialKey(const ResourceHandle &_programHandle, const ResourceHandle &_textureHandle) :
					programHandle(_programHandle), textureHandle(_textureHandle)
				{};

				bool operator==(const MaterialKey &_rhs) const
				{
					return programHandle == _rhs.programHandle && textureHandle == _rhs.textureHandle;
				};

				struct Hash
				{
					size_t operator()(const MaterialKey &_pair) const noexcept
					{
						return (size_t)(_pair.programHandle ^ (_pair.textureHandle << 8));
					};
				};

			};


			typedef std::unordered_map<MaterialKey, MaterialPtr, MaterialKey::Hash> MaterialIdList;
			typedef std::unordered_map<MaterialId, MaterialPtr> MaterialDefByIdList;

			MaterialIdList materialIdsList;
			MaterialDefByIdList materialsByIds;

			MaterialId nextFreeId;

		public:

			MaterialManager();

			MaterialPtr generateMaterial(ShadingProgramPtr _program, TexturePtr _texture);
			MaterialPtr getById(MaterialId _id);

		};
	}
}
