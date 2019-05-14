#pragma once

#include <vector>
#include "Texture2D.h"
#include "Program.h"



namespace Graphic
{

	class Material
	{
	private:
		std::string name;

		PProgram program;
		std::vector<std::pair<GLint, PTexture2D>> textures;

	public:

		
		const std::string& getName() const
		{
			return name;
		};


		int getTexturesCount() const
		{
			return textures.size();
		};

		const std::string& getTextureName(int index) const
		{
			return textures.at(index).second->getName();
		};

		const PTexture2D getTexture(int index) const
		{
			return textures.at(index).second;
		};


		inline GLint getUniformId(const std::string& uniformName)
		{
			return program->getUniformId(uniformName);
		};
	};

	typedef std::shared_ptr<Material> PMaterial;


}
