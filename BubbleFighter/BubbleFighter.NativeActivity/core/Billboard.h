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

	union
	{
		float  position[4];
		struct
		{
			float top, left, bottom, right;
		};
	};


public:

	Billboard()
		: top(0.f), left(0.f), bottom(0.f), right(0.f)
	{};

	Billboard(const std::string& _name, PMaterial _material, float _top, float _left, float _bottom, float _right)
		: name(_name), material(_material), top(_top), left(_left), bottom(_bottom), right(_right)
	{};

	const std::string& getName() const
	{
		return name;
	};

	inline const float* getPosition() const
	{
		return position;
	};
};

typedef std::shared_ptr<Billboard> PBillboard;

