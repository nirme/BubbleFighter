#include "utils\vector.h"


Vec2f& normalize(Vec2f& _v)
{
	return _v *= (1.f / length(_v));
};

Vec3f& normalize(Vec3f& _v)
{
	return _v *= (1.f / length(_v));
};

Vec4f& normalize(Vec4f& _v)
{
	return _v *= (1.f / length(_v));
};


Vec2f normalize(const Vec2f& _v)
{
	Vec2f out(_v);
	return normalize(out);
};

Vec3f normalize(const Vec3f& _v)
{
	Vec3f out(_v);
	return normalize(out);
};

Vec4f normalize(const Vec4f& _v)
{
	Vec4f out(_v);
	return normalize(out);
};


Vec3f crossProduct(const Vec3f& _v1, const Vec3f& _v2)
{
	return Vec3f(_v1.v2 * _v2.v3 - _v1.v3 * _v2.v2,
		_v1.v3 * _v2.v1 - _v1.v1 * _v2.v3,
		_v1.v1 * _v2.v2 - _v1.v2 * _v2.v1);
};

