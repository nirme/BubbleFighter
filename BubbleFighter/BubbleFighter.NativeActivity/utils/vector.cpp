#include "utils\vector.h"



float lengthSquared(const Vec2f& _v)
{
	return _v.v1 * _v.v1 + _v.v2 * _v.v2;
};

float lengthSquared(const Vec3f& _v)
{
	return _v.v1 * _v.v1 + _v.v2 * _v.v2 + _v.v3 * _v.v3;
};

float lengthSquared(const Vec4f& _v)
{
	return _v.v1 * _v.v1 + _v.v2 * _v.v2 + _v.v3 * _v.v3 + _v.v4 * _v.v4;
};



float length(const Vec2f& _v)
{
	return sqrtf(lengthSquared(_v));
};

float length(const Vec3f& _v)
{
	return sqrtf(lengthSquared(_v));
};

float length(const Vec4f& _v)
{
	return sqrtf(lengthSquared(_v));
};



float distance(const Vec2f& _v1, const Vec2f& _v2)
{
	return length(_v2 - _v1);
};

float distance(const Vec3f& _v1, const Vec3f& _v2)
{
	return length(_v2 - _v1);
};

float distance(const Vec4f& _v1, const Vec4f& _v2)
{
	return length(_v2 - _v1);
};



Vec2f orthogonalCounterClockwise(const Vec2f& _direction)
{
	return Vec2f(-_direction.y, _direction.x);
};

Vec2f orthogonalClockwise(const Vec2f& _direction)
{
	return Vec2f(_direction.y, -_direction.x);
};



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



float dotProduct(const Vec2f& _v1, const Vec2f& _v2)
{
	return _v1.v1 * _v2.v1 + _v1.v2 * _v2.v2;
};

float dotProduct(const Vec3f& _v1, const Vec3f& _v2)
{
	return _v1.v1 * _v2.v1 + _v1.v2 * _v2.v2 + _v1.v3 * _v2.v3;
};

float dotProduct(const Vec4f& _v1, const Vec4f& _v2)
{
	return _v1.v1 * _v2.v1 + _v1.v2 * _v2.v2 + _v1.v3 * _v2.v3 + _v1.v4 * _v2.v4;
};



Vec3f crossProduct(const Vec3f& _v1, const Vec3f& _v2)
{
	return Vec3f(_v1.v2 * _v2.v3 - _v1.v3 * _v2.v2,
		_v1.v3 * _v2.v1 - _v1.v1 * _v2.v3,
		_v1.v1 * _v2.v2 - _v1.v2 * _v2.v1);
};

