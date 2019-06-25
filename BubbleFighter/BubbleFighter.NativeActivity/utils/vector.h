#pragma once

#include <cmath>
#include "TVector.h"


typedef TVec2<int> Vec2i;
typedef TVec3<int> Vec3i;
typedef TVec4<int> Vec4i;

typedef TVec2<float> Vec2f;
typedef TVec3<float> Vec3f;
typedef TVec4<float> Vec4f;



inline float lengthSquared(const Vec2f& _v)
{
	return _v.v1 * _v.v1 + _v.v2 * _v.v2;
};

inline float lengthSquared(const Vec3f& _v)
{
	return _v.v1 * _v.v1 + _v.v2 * _v.v2 + _v.v3 * _v.v3;
};

inline float lengthSquared(const Vec4f& _v)
{
	return _v.v1 * _v.v1 + _v.v2 * _v.v2 + _v.v3 * _v.v3 + _v.v4 * _v.v4;
};



inline float length(const Vec2f& _v)
{
	return sqrtf(lengthSquared(_v));
};

inline float length(const Vec3f& _v)
{
	return sqrtf(lengthSquared(_v));
};

inline float length(const Vec4f& _v)
{
	return sqrtf(lengthSquared(_v));
};



inline float distance(const Vec2f& _v1, const Vec2f& _v2)
{
	return length(_v2 - _v1);
};

inline float distance(const Vec3f& _v1, const Vec3f& _v2)
{
	return length(_v2 - _v1);
};

inline float distance(const Vec4f& _v1, const Vec4f& _v2)
{
	return length(_v2 - _v1);
};



inline Vec2f orthogonalCounterClockwise(const Vec2f& _direction)
{
	return Vec2f(-_direction.y, _direction.x);
};



Vec2f& normalize(Vec2f& _v);
Vec3f& normalize(Vec3f& _v);
Vec4f& normalize(Vec4f& _v);

Vec2f normalize(const Vec2f& _v);
Vec3f normalize(const Vec3f& _v);
Vec4f normalize(const Vec4f& _v);


inline float dotProduct(const Vec2f& _v1, const Vec2f& _v2)
{
	return _v1.v1 * _v2.v1 + _v1.v2 * _v2.v2;
};

inline float dotProduct(const Vec3f& _v1, const Vec3f& _v2)
{
	return _v1.v1 * _v2.v1 + _v1.v2 * _v2.v2 + _v1.v3 * _v2.v3;
};

inline float dotProduct(const Vec4f& _v1, const Vec4f& _v2)
{
	return _v1.v1 * _v2.v1 + _v1.v2 * _v2.v2 + _v1.v3 * _v2.v3 + _v1.v4 * _v2.v4;
};


Vec3f crossProduct(const Vec3f& _v1, const Vec3f& _v2);

