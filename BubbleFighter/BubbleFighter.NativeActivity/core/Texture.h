#pragma once


namespace Graphic
{

	class Renderer
	{
	public:
		virtual void createMaterial() = 0;
		virtual void deleteMaterial() = 0;
		virtual void createTexture() = 0;
		virtual void deleteTexture() = 0;
		virtual void createShader() = 0;
		virtual void deleteShader() = 0;
		virtual void createProgram() = 0;
		virtual void deleteProgram() = 0;
	};




	class Texture
	{
	public:
		void getWidth();
		void getHeight();
	};



	class Shader
	{
	public:

	};

	class Program
	{
	public:

	};


	class Material
	{
	};


	class RenderChunk
	{
		private Material* material;


	};

}