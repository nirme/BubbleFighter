#pragma once

#include <memory>
#include <functional>

#include <vector>
#include <list>
#include <unordered_map>


#include <GLES2/gl2.h>


#include "Texture.h"



namespace Graphic
{

	//typedef std::shared_ptr<

	class Renderer
	{

	private:


	public:
		virtual PTexture2D loadTexture() = 0;
		virtual void unloadTexture(Texture2D* texture) = 0;

		virtual void loadShader() = 0;
		virtual void unloadShader() = 0;
		virtual void loadProgram() = 0;
		virtual void unloadProgram() = 0;
		virtual void loadMaterial() = 0;
		virtual void unloadMaterial() = 0;
	};







	class Material
	{
	};


	class RenderChunk
	{
	private:
		Material* material;

	};

}