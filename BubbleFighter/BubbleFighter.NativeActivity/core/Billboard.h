#pragma once

#include <vector>
#include <unordered_map>

#include "Material.h"
#include "vec.h"


using namespace Graphic;


class Billboard
{
private:
	std::string name;

	PMaterial material;

	std::vector<Vec2f> positionOnTextures;
	// texture positions storage plan:
	// v0(t0.uv, t1.uv, t2.uv), v1(t0.uv, t1.uv, t2.uv), v2(t0.uv, t1.uv, t2.uv), v3(t0.uv, t1.uv, t2.uv)

	Vec2f size;
	Vec2f offset;
	// offset is applied after scaling (+ to billboard quad verts), no offset means billbord quad is:
	// v0(-0.5, 0.5)  +---+  v1(0.5, 0.5)
	//                | \ |
	// v2(-0.5,-0.5)  +---+  v3(0.5,-0.5)

	inline static const Vec2f billboardBase[4] = { Vec2f(-0.5, 0.5), Vec2f(0.5, 0.5), Vec2f(-0.5,-0.5), Vec2f(0.5,-0.5) };

public:

	typedef enum BILLBOARD_VERTICES
	{
		BV_TOP_LEFT = 0,
		BV_TOP_RIGHT = 1,
		BV_BOTTOM_RIGHT = 2,
		BV_BOTTOM_LEFT = 3,
	};

	Billboard(const std::string& _name, PMaterial _material)
		: name(_name), material(_material), positionOnTextures(material->getTextureCount()), size(1.f, 1.f), offset(0.f, 0.f)
	{};


	Billboard(const std::string& _name, PMaterial _material, const std::vector<Vec2f>& _positionOnTextures, Vec2f _size, Vec2f _offset)
		: name(_name), material(_material), positionOnTextures(_positionOnTextures), size(_size), offset(_offset)
	{};


	void setTextureCoords(BILLBOARD_VERTICES vertex, unsigned short textureIndex, Vec2f coords)
	{
		positionOnTextures[(unsigned short)vertex * material->getTextureCount() + textureIndex] = coords;
	};

	void setSize(Vec2f _size)
	{
		size = _size;
	};

	void setOffset(Vec2f _offset)
	{
		offset = _offset;
	};


	inline int getMaterialId() const
	{
		return material->getId();
	};

	const std::string& getName() const
	{
		return name;
	};


	inline const Material* getMaterialPtr() const
	{
		return material.get();
	};


	inline const Vec2f& getSize() const								{	return size;	};
	inline const Vec2f& getOffset() const							{	return offset;	};
	inline const std::vector<Vec2f>& getPositionOnTextures() const	{	return positionOnTextures;	};


	inline unsigned short getVertexSize() const
	{
		// there are 2 floats per texture (u,v) + 2 floats for vertex (x,y) / z wil be added in shader
		return (material->getTextureCount() + 1) * sizeof(float) * 2;
	};


	inline void fillCombinedVertexData(float* vOutput) const
	{
		assert(vOutput != nullptr);

		unsigned short textureCount = material->getTextureCount();
		unsigned short vLength = (textureCount + 1) * 2;
		unsigned short vTexSizeBytes = textureCount * sizeof(Vec2f);

		const Vec2f* texCoordsData = positionOnTextures.data();

		int i = 0;
		i = 0;

		// unrolled version in use
		// vOutput[i * vLength + 0] = billboardBase[i].x * size.x + offset.x;
		// vOutput[i * vLength + 1] = billboardBase[i].y * size.y + offset.y;
		// memcpy(&(vOutput[i * vLength + 2]), (texCoordsData + (i * textureCount)), vTexSizeBytes);

		vOutput[0] = billboardBase[0].x * size.x + offset.x;
		vOutput[1] = billboardBase[0].y * size.y + offset.y;
		memcpy(&(vOutput[2]), texCoordsData, vTexSizeBytes);

		vOutput[vLength] = billboardBase[1].x * size.x + offset.x;
		vOutput[vLength + 1] = billboardBase[1].y * size.y + offset.y;
		memcpy(&(vOutput[vLength + 2]), (texCoordsData + textureCount), vTexSizeBytes);

		vOutput[2 * vLength] = billboardBase[2].x * size.x + offset.x;
		vOutput[2 * vLength + 1] = billboardBase[2].y * size.y + offset.y;
		memcpy(&(vOutput[2 * vLength + 2]), (texCoordsData + (2 * textureCount)), vTexSizeBytes);

		vOutput[3 * vLength] = billboardBase[3].x * size.x + offset.x;
		vOutput[3 * vLength + 1] = billboardBase[3].y * size.y + offset.y;
		memcpy(&(vOutput[3 * vLength + 2]), (texCoordsData + (3 * textureCount)), vTexSizeBytes);
	};


	inline void fillSeparatedVertexData(float* vPositions, float* vTexCoords) const
	{
		assert(vPositions != nullptr && vTexCoords != nullptr);

		vPositions[0] = billboardBase[0].x * size.x + offset.x;
		vPositions[1] = billboardBase[0].y * size.y + offset.y;
		vPositions[2] = billboardBase[1].x * size.x + offset.x;
		vPositions[3] = billboardBase[1].y * size.y + offset.y;
		vPositions[4] = billboardBase[2].x * size.x + offset.x;
		vPositions[5] = billboardBase[2].y * size.y + offset.y;
		vPositions[6] = billboardBase[3].x * size.x + offset.x;
		vPositions[7] = billboardBase[3].y * size.y + offset.y;

		memcpy(vTexCoords, positionOnTextures.data(), positionOnTextures.size() * sizeof(float));
	};


};

typedef std::shared_ptr<Billboard> PBillboard;

