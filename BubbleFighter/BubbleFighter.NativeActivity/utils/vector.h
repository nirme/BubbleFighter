#pragma once

#include <cmath>
#include "TVector.h"


typedef TVec2<int> Vec2i;
typedef TVec3<int> Vec3i;
typedef TVec4<int> Vec4i;

typedef TVec2<float> Vec2f;
typedef TVec3<float> Vec3f;
typedef TVec4<float> Vec4f;


float lengthSquared(const Vec2f& _v);
float lengthSquared(const Vec3f& _v);
float lengthSquared(const Vec4f& _v);

float length(const Vec2f& _v);
float length(const Vec3f& _v);
float length(const Vec4f& _v);

float distance(const Vec2f& _v1, const Vec2f& _v2);
float distance(const Vec3f& _v1, const Vec3f& _v2);
float distance(const Vec4f& _v1, const Vec4f& _v2);

Vec2f orthogonalCounterClockwise(const Vec2f& _direction);
Vec2f orthogonalClockwise(const Vec2f& _direction);

Vec2f& normalize(Vec2f& _v);
Vec3f& normalize(Vec3f& _v);
Vec4f& normalize(Vec4f& _v);

Vec2f normalize(const Vec2f& _v);
Vec3f normalize(const Vec3f& _v);
Vec4f normalize(const Vec4f& _v);

float dotProduct(const Vec2f& _v1, const Vec2f& _v2);
float dotProduct(const Vec3f& _v1, const Vec3f& _v2);
float dotProduct(const Vec4f& _v1, const Vec4f& _v2);

Vec3f crossProduct(const Vec3f& _v1, const Vec3f& _v2);

