#pragma once


#include "Matrix3.h"
#include "Vector2.h"
#include "_2d/Quaternion.h"


namespace core
{

	//  by default V2 is treated like point in 2D
	inline Vector2 operator*(const Matrix3& _m, const Vector2& _v)
	{
		return Vector2(
			_m.m11 * _v.x + _m.m12 * _v.y + _m.m13,
			_m.m21 * _v.x + _m.m22 * _v.y + _m.m23
		);
	};


	//  transform 2D vector
	inline Vector2 transformVector(const Matrix3& _m, const Vector2& _v)
	{
		return Vector2(
			_m.m11 * _v.x + _m.m12 * _v.y,
			_m.m21 * _v.x + _m.m22 * _v.y
		);
	};


	//  default transformation goes scaling -> rotation -> translation
	inline Matrix3 affine2DMatrix(Vector2 _scale, _2d::Quaternion _rotation, Vector2 _translation)
	{
		//  mx from quaternion
		//  1.0f - (2 * z * z)		2 * z * w				0
		//  -2 * z * w				1.0f - (2 * z * z)		0
		//  0						0						1

		//  full matrix
		//	Sx * rc		Sy * rs		tx
		//	Sx * -rs	Sy * rc		ty
		//	0			0			1


		float rc = 1.0f - (2.0f * _rotation.z * _rotation.z);
		float rs = 2.0f * _rotation.z * _rotation.w;

		return Matrix3(
			_scale.x * rc,		_scale.y * rs,		_translation.x,
			_scale.x * -rs,		_scale.y * rc,		_translation.y,
			0.0f,				0.0f,				1.0f
		);

	};

}