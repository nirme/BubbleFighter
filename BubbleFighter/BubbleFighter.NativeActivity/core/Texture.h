#pragma once





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
	virtual void getWidth() = 0;
	virtual void getHeight() = 0;
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
