#pragma once

#include <vector>

#include "VideoDriver.h"


#include "Texture2D.h"
#include "Program.h"



namespace Graphic
{
	class Material
	{
	private:
		int localId;
		std::string name;

		PProgram program;
		std::vector<std::pair<unsigned short, PTexture2D>> texturesWithUnitIndex;




	public:


		void apply(VideoDriver* vDriver) const;

		inline int getId() const
		{
			return localId;
		};
		
		const std::string& getName() const
		{
			return name;
		};

		inline GLuint getProgramId() const
		{
			return program->getId();
		};

		inline GLint getUniformId(const std::string& uniformName)
		{
			return program->getUniformId(uniformName);
		};

		inline unsigned short getTextureCount() const
		{
			return (unsigned short) texturesWithUnitIndex.size();
		};


	};

	typedef std::shared_ptr<Material> PMaterial;


}
