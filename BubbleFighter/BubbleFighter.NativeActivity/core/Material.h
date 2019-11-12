#pragma once

#include "Resource.h"
#include "ShadingProgram.h"
#include "Texture.h"

#include "ShadingProgramManager.h"
#include "TextureManager.h"


namespace core
{
	typedef ResourceHandle MaterialId;

	class Material : public Resource
	{
	protected:

		MaterialId id;

		std::string programName;
		ShadingProgramPtr program;

		struct NamedTexture
		{
			std::string name;
			TexturePtr texture;
		};

		std::vector<NamedTexture> textures;



	public:

		Material(const std::string &_name, ResourceHandle _handle, const std::string &_group, ResourceManager *_manager, MaterialId _materialId) :
			Resource(_name, _handle, _group, _manager),
			id(_materialId)
		{};



		void loadImp()
		{
			if (!program)
				program = ShadingProgramManager::getSingleton().getByName(programName, group);
			program->load();

			for (unsigned int i = 0; i < textures.size(); ++i)
			{
				if (!textures[i].texture)
					textures[i].texture = TextureManager::getSingleton().getByName(textures[i].name, group);
				textures[i].texture->load();
			}
		};

		void unloadImp()
		{
			program.reset();

			for (unsigned int i = 0; i < textures.size(); ++i)
				textures[i].texture.reset();
		};

		unsigned int sizeCalcImpl()
		{
			unsigned int s = sizeof(programName) + programName.size();
			for (unsigned int i = 0; i < textures.size(); ++i)
				s += sizeof(textures[i].name) + textures[i].name.size();

			return s;
		};



		void setProgram(const std::string &_name)
		{
			assert(isLoaded() && "Cannot change already loaded Program");
			programName = _name;
		};

		void setTexture(unsigned char _index, const std::string &_name)
		{
			assert(isLoaded() && "Cannot change already loaded Texture");

			if (textures.size() <= _index)
			{
				textures.resize(_index + 1);
				textures.shrink_to_fit();
			}

			textures[_index].name = _name;
		};


	};

	typedef std::shared_ptr<Material> MaterialPtr;

}
