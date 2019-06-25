#pragma once

#include <cmath>
#include "utils\vector.h"


class Mx2d
{
public:
	union
	{
		float _m[4];
		struct
		{
			float	m11, m12,
				m21, m22;
		};
	};

	Mx2d();
	Mx2d(float _m11, float _m12, float _m21, float _m22);
	Mx2d(const Mx2d& _m);

	inline Vec2f& mul(const Vec2f& _v, Vec2f& _out) const
	{
		_out.v1 = m11 * _v.v1 + m12 * _v.v2;
		_out.v2 = m21 * _v.v1 + m22 * _v.v2;
		return _out;
	}

	Vec2f mul(const Vec2f& _v) const;
	Vec2f operator *(const Vec2f& _v) const;

	inline Mx2d& mul(const Mx2d& _m, Mx2d& _out) const
	{
		_out.m11 = m11 * _m.m11 + m12 * _m.m21;
		_out.m12 = m11 * _m.m12 + m12 * _m.m22;
		_out.m21 = m21 * _m.m11 + m22 * _m.m21;
		_out.m22 = m21 * _m.m12 + m22 * _m.m22;
		return _out;
	}

	Mx2d mul(const Mx2d& _m) const;
	Mx2d operator *(Mx2d _m) const;
};


class Mx3d
{
public:
	union
	{
		float _m[9];
		struct
		{
			float	m11, m12, m13,
				m21, m22, m23,
				m31, m32, m33;
		};
	};



	Mx3d();

	Mx3d(float _m11, float _m12, float _m13, float _m21, float _m22, float _m23, float _m31, float _m32, float _m33);
	Mx3d(const Mx3d& _m);

	inline Vec3f& mul(const Vec3f& _v, Vec3f& _out) const
	{
		_out.v1 = m11 * _v.v1 + m12 * _v.v2 + m13 * _v.v3;
		_out.v2 = m21 * _v.v1 + m22 * _v.v2 + m23 * _v.v3;
		_out.v3 = m31 * _v.v1 + m32 * _v.v2 + m33 * _v.v3;

		return _out;
	}

	Vec3f mul(const Vec3f& _v) const;
	Vec3f operator *(const Vec3f& _v) const;

	inline Vec2f& mul(const Vec2f& _v, Vec2f& _out) const
	{
		_out.v1 = m11 * _v.v1 + m12 * _v.v2 + m13;
		_out.v2 = m21 * _v.v1 + m22 * _v.v2 + m23;

		return _out;
	}

	Vec2f mul(const Vec2f& _v) const;
	Vec2f operator *(const Vec2f& _v) const;


	inline Mx3d& mul(const Mx3d& _m, Mx3d& _out) const
	{
		_out.m11 = m11 * _m.m11 + m12 * _m.m21 + m13 * _m.m31;
		_out.m12 = m11 * _m.m12 + m12 * _m.m22 + m13 * _m.m32;
		_out.m13 = m11 * _m.m13 + m12 * _m.m23 + m13 * _m.m33;

		_out.m21 = m21 * _m.m11 + m22 * _m.m21 + m23 * _m.m31;
		_out.m22 = m21 * _m.m12 + m22 * _m.m22 + m23 * _m.m32;
		_out.m23 = m21 * _m.m13 + m22 * _m.m23 + m23 * _m.m33;

		_out.m31 = m31 * _m.m11 + m32 * _m.m21 + m33 * _m.m31;
		_out.m32 = m31 * _m.m12 + m32 * _m.m22 + m33 * _m.m32;
		_out.m33 = m31 * _m.m13 + m32 * _m.m23 + m33 * _m.m33;

		return _out;
	}

	Mx3d mul(const Mx3d& _m) const;
	Mx3d operator *(const Mx3d& _m) const;
};



Mx2d& identity(Mx2d& _m);
Mx3d& identity(Mx3d& _m);

Mx3d& mxTranslate(Mx3d& _m, float _translateX, float _translateY);
Mx3d& mxRotate(Mx3d& _m, float _rotationAngle);
Mx3d& mxRotate(Mx3d& _m, float _rotationAngle, const Vec2f& _rotationPoint);
Mx3d& mxScale(Mx3d& _m, float _scaleX, float _scaleY);
Mx3d& mxScaleRotateTranslate(Mx3d& _m, float _scaleX, float _scaleY, float _rotationAngle, float _translateX, float _translateY);
Mx3d& mxScaleTranslate(Mx3d& _m, float _scaleX, float _scaleY, float _translateX, float _translateY);

