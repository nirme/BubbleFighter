#pragma once

#include <memory>
#include <functional>

#include <vector>
#include <list>
#include <unordered_map>


#include <GLES2/gl2.h>


#include "Texture2D.h"
#include "Shader.h"
#include "Program.h"
#include "Material.h"



namespace Graphic
{
	class Renderer
	{

	private:




	public:
		virtual PTexture2D loadTexture() = 0;
		virtual void unloadTexture(Texture2D* texture) = 0;

		virtual PShader loadShader() = 0;
		virtual void unloadShader(Shader* shader) = 0;

		virtual PProgram loadProgram() = 0;
		virtual void unloadProgram(Program* program) = 0;

		virtual PMaterial loadMaterial() = 0;
		virtual void unloadMaterial(Material* material) = 0;
	};





	class RenderChunk2D
	{
	private:
		Material* material;



	};

}
